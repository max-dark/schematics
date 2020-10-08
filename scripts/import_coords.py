# -*- coding: utf-8 -*-
from typing import Dict
from xml.etree import ElementTree as ET

from PySide2.QtSql import *


def to_address(byte: int, bit: int = None) -> str:
    if bit is None:
        return f"M0:{byte}"
    return f"M0:{byte}.{bit}"


def load_bits(name: str) -> Dict[int, str]:
    bits = dict()

    doc = ET.parse(name)
    root = doc.getroot()
    for bit in root.findall('bit'):
        bit_id = int(bit.get('id'))
        byte_no = int(bit.get('address'))
        bit_no = int(bit.get('bit'))
        # desc = str(bit.get('description'))
        addr_str = to_address(byte_no, bit_no)
        # print(bit_id, addr_str, desc)
        bits[bit_id] = addr_str

    return bits


def load_coords(name: str) -> Dict[int, str]:
    coords = dict()

    doc = ET.parse(name)
    root = doc.getroot()
    for coord in root.findall('coord'):
        cid = int(coord.get('id'))
        byte_no = int(coord.get('address'))
        # desc = str(coord.get('description'))
        addr_str = to_address(byte_no)
        # print(cid, addr_str, desc)
        coords[cid] = addr_str

    return coords


def save_to(db_name: str, address: Dict[int, str], apply_bit: Dict[int, str]) -> None:
    db: QSqlDatabase = QSqlDatabase.addDatabase('QSQLITE')
    db.setDatabaseName(db_name)
    if not db.open():
        print("cant open db")
        return

    db.transaction()
    update = QSqlQuery(db)
    ok = update.prepare('update offsets set address = :address, apply_bit = :apply_bit where id = :id')
    if not ok:
        print('error in sql')
        db.rollback()
        return

    for cid in address:
        coord = address[cid]
        apply = apply_bit[cid]
        print(cid, coord, apply)
        update.bindValue(':id', cid)
        update.bindValue(':address', coord)
        update.bindValue(':apply_bit', apply)
        ok = update.exec_()
        if not ok:
            db.rollback()
            print("error while update::exec")
            return

    db.commit()
    print('offsets update done')


if __name__ == '__main__':
    BITS_FILE = 'xml-config/coordsbits.xml'
    COORD_FILE = 'xml-config/coords.xml'
    DB_FILE = 'test.db'
    bits = load_bits(BITS_FILE)
    coords = load_coords(COORD_FILE)

    save_to(DB_FILE, coords, bits)
    pass
