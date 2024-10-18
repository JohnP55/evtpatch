import argparse
from dataclasses import dataclass
from typing import List

class BinaryReader:
    def __init__(self, file, base_addr = 0x8000_0000):
        self._f = file
        self._ba = base_addr
    
    def read_word(self, addr):
        offs = addr - self._ba
        self._f.seek(offs)
        dat = self._f.read(4)
        return int.from_bytes(dat, 'big')

@dataclass
class RelSection:
    addr: int
    size: int
    
@dataclass
class Rel:
    moduleId: int
    sections: List[RelSection]

    def find_loc(self, addr):
        for i, sec in enumerate(self.sections):
            if sec.addr <= addr < sec.addr + sec.size:
                return i, addr - sec.addr
        return None

    def from_ram(ram, rel_p):
        moduleId = ram.read_word(rel_p + 0x0)
        sectionCount = ram.read_word(rel_p + 0xc)
        sec_p = ram.read_word(rel_p + 0x10)
        sections = []
        for _ in range(sectionCount):
            addr = ram.read_word(sec_p + 0x0) & ~3
            size = ram.read_word(sec_p + 0x4)

            sections.append(RelSection(addr, size))

            sec_p += 8

        return Rel(moduleId, sections)

def get_rels(ram):
    ret = []
    rel_p = ram.read_word(0x800030C8)

    while rel_p != 0:
        ret.append(Rel.from_ram(ram, rel_p))
        rel_p = ram.read_word(rel_p + 0x4)

    return ret

def find_loc(rels, addr):
    for rel in rels:
        ret = rel.find_loc(addr)
        if ret is not None:
            return rel.moduleId, *ret
    assert 0, "Address not found in any loaded rel"

if __name__=="__main__":
    hex_int = lambda x: int(x, 16)
    parser = argparse.ArgumentParser(description="Convert ram addresses to rel offsets")
    parser.add_argument("ram_path", type=str, help="Ram dump input path")
    parser.add_argument("address", type=hex_int, help="Ram address to convert")
    args = parser.parse_args()

    with open(args.ram_path, 'rb') as ram:
        ram = BinaryReader(ram)
        rels = get_rels(ram)
    moduleId, sectionId, offset = find_loc(rels, args.address)
    print(f"{moduleId:x},{sectionId:x},{offset:08x}")
