# -*- coding: utf-8 -*-
from collections import namedtuple
from typing import Dict
from xml.etree import ElementTree as Et

Bit = namedtuple('Bit', ['address', 'description'])
Num = namedtuple('Num', ['address', 'base', 'offset', 'description'])
BitList = Dict[int, Bit]
NumList = Dict[int, Num]


class FormatException(Exception):
    pass


class Config:
    alarms: BitList = {}
    motors: BitList = {}
    sensors: BitList = {}

    delays: NumList = {}
    speeds: NumList = {}

    def __str__(self):
        return f"""Config[
    {self.alarms},
    {self.motors},
    {self.sensors},
    {self.delays},
    {self.speeds}
]"""

    def __repr__(self):
        return self.__str__()


def convert_tag(tag: Et.Element) -> str:
    var_type = tag.get('vartype')
    bit_code: str = ''
    if var_type == "1":
        bit = int(tag.get('bit'))
        if bit not in range(8):
            raise FormatException(f"Bit = {bit}.  It must be in range[0..7]")
        bit_code = f'.{bit}'

    area_name = tag.get('area', 'flag')
    area_code: str
    db_code: int = 0
    byte_no = int(tag.get('address'))

    if area_name == 'input':
        area_code = 'I'
    elif area_name == 'output':
        area_code = 'Q'
    elif area_name == 'flag':
        area_code = 'M'
    else:
        raise FormatException(f"Unknown area: {area_name}")

    return f'{area_code}{db_code}:{byte_no}{bit_code}'


def parse_bits(file: str) -> Dict[int, Bit]:
    xml = Et.parse(file)
    root = xml.getroot()
    bits: Dict[int, Bit] = {}

    for bit in root.findall('bit'):
        bit_id = int(bit.get('id'))
        text = bit.get('description')

        bits[bit_id] = Bit(convert_tag(bit), text)

    return bits


def parse_nums(file: str, name: str) -> Dict[int, Num]:
    xml = Et.parse(file)
    root = xml.getroot()

    nums: Dict[int, Num] = {}

    for num in root.findall(name):
        num_id = int(num.get('id'))
        num_value = int(num.get('base'))
        num_offset = int(num.get(key='shift', default='0'))
        num_text = num.get('description')
        num_addr = convert_tag(num)

        nums[num_id] = Num(num_addr, num_value, num_offset, num_text)

    return nums


def load_config(cfg_dir: str) -> Config:
    from pathlib import Path
    cfg = Path(cfg_dir).absolute()
    config = Config()
    config.alarms = parse_bits(cfg / 'alarm.xml')
    config.sensors = parse_bits(cfg / 'sensors.xml')
    config.motors = parse_bits(cfg / 'engines.xml')
    config.delays = parse_nums(cfg / 'delays.xml', 'delay')
    config.speeds = parse_nums(cfg / 'speeds.xml', 'speed')

    return config