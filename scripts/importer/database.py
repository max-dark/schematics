# -*- coding: utf-8 -*-
from PyQt5.QtSql import *
from PyQt5.QtCore import \
    pyqtSlot as Slot
from PyQt5.QtWidgets import \
    QDialog, QWidget, QFormLayout, \
    QLineEdit, QPlainTextEdit, QPushButton, \
    QFileDialog, QLayout

from importer.parser import Config, load_config, NumList, BitList


class DatabaseDialog(QDialog):
    def __init__(self, parent: QWidget = None):
        super(DatabaseDialog, self).__init__(parent=parent)
        self.setLayout(self.__create_ui())

    def __create_ui(self) -> QLayout:
        self.__db_name = QLineEdit()
        self.__cfg_dir = QLineEdit()
        self.__db_name.setReadOnly(True)
        self.__cfg_dir.setReadOnly(True)
        self.__log = QPlainTextEdit()
        self.__log.setReadOnly(True)

        btn_db = QPushButton("Select DB")
        btn_cfg = QPushButton("Select CFG")
        btn_import = QPushButton("run import")
        self.__btn_import = btn_import

        # connect actions
        self.__db_name.textChanged.connect(self.__allow_import)
        self.__cfg_dir.textChanged.connect(self.__allow_import)
        btn_db.clicked.connect(self.__select_db)
        btn_cfg.clicked.connect(self.__select_cfg)
        btn_import.clicked.connect(self.__import_configs)

        # fill view

        box = QFormLayout()
        box.addRow("SQLite DB:", self.__db_name)
        box.addWidget(btn_db)
        box.addRow("Config dir", self.__cfg_dir)
        box.addWidget(btn_cfg)
        box.addWidget(self.__log)
        box.addWidget(btn_import)

        return box

    @Slot()
    def __allow_import(self):
        allow = (len(self.__db_name.text()) > 0) and \
                (len(self.__cfg_dir.text()) > 0)
        self.__btn_import.setEnabled(allow)

    @Slot()
    def __select_db(self):
        (name, _) = QFileDialog.getOpenFileName(self)
        self.__db_name.setText(name)

    @Slot()
    def __select_cfg(self):
        name = QFileDialog.getExistingDirectory(self)
        self.__cfg_dir.setText(name)

    @Slot()
    def __import_configs(self):
        config: Config = load_config(self.__cfg_dir.text())
        self.__log.clear()

        db = QSqlDatabase.addDatabase('QSQLITE')
        db.setDatabaseName(self.__db_name.text())

        if db.open():
            log = 'run import:\n'
            log += '\talarms ' + import_bits(db, 'alarms', config.alarms)
            log += '\tsensors ' + import_bits(db, 'sensors', config.sensors)
            log += '\tmotors ' + import_bits(db, 'motors', config.motors)
            log += '\tdoors ' + import_bits(db, 'doors', config.doors)
            log += '\tdelays ' + import_nums(db, 'delays', config.delays)
            log += '\tspeeds ' + import_nums(db, 'speeds', config.speeds)

            db.close()

            self.__log.setPlainText(log)
        else:
            self.__log.setPlainText('Open db failure: ' + db.lastError().text())


def import_bits(db: QSqlDatabase, name: str, bits: BitList):
    db.transaction()

    insert = QSqlQuery(db)
    ok = insert.prepare(f'insert into {name} ("id", "address", "description") values(:id, :address, :description)')
    if ok:
        for bit_id, bit in bits.items():
            insert.bindValue(':id', bit_id)
            insert.bindValue(':address', bit.address)
            insert.bindValue(':description', bit.description)

            if not insert.exec_():
                break
        else:
            db.commit()
            return "OK\n"

    db.rollback()

    return insert.lastError().text() + "\n"


def import_nums(db: QSqlDatabase, name: str, nums: NumList):
    db.transaction()

    insert = QSqlQuery(db)
    ok = insert.prepare(f'insert into {name}'
                        f' ("id", "address", "base", "offset", "description")'
                        f' values(:id, :address, :base, :offset, :description)')
    if ok:
        for num_id, num in nums.items():
            insert.bindValue(':id', num_id)
            insert.bindValue(':address', num.address)
            insert.bindValue(':base', num.base)
            insert.bindValue(':offset', num.offset)
            insert.bindValue(':description', num.description)

            if not insert.exec_():
                break
        else:
            db.commit()
            return "OK"

    db.rollback()

    return insert.lastError().text()