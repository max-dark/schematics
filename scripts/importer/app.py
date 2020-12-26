# -*- coding: utf-8 -*-
from typing import List

from PyQt5.QtWidgets import QApplication

from importer.views import MainWindow


def main(args: List[str]) -> int:
    app = QApplication(args)
    window = MainWindow()
    window.show()
    return app.exec_()
