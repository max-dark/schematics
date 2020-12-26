# -*- coding: utf-8 -*-
from PyQt5.QtCore import \
    pyqtSlot as Slot, \
    pyqtSignal as Signal
from PyQt5.QtWidgets import \
    QMainWindow, QWidget, \
    QMenuBar, QMenu, QAction

from importer.database import DatabaseDialog


class MainWindow(QMainWindow):
    def __init__(self, parent: QWidget = None):
        super().__init__(parent=parent)

        self.__central = QWidget()

        self.setCentralWidget(self.__central)
        self.__create_ui()

    def __create_ui(self):
        menu_bar = QMenuBar()
        test_menu: QMenu = menu_bar.addMenu("tests")
        test_db: QAction = test_menu.addAction("Test DB")
        test_db.triggered.connect(self.__test_db)
        test_menu.addSeparator()
        test_exit: QAction = test_menu.addAction("Exit")
        test_exit.triggered.connect(self.__close)
        self.setMenuBar(menu_bar)

    @Slot()
    def __test_db(self):
        dlg = DatabaseDialog(self)
        dlg.open()

    @Slot()
    def __close(self):
        self.close()
