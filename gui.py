# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'gui.ui'
#
# Created by: PyQt4 UI code generator 4.11.4
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    def _fromUtf8(s):
        return s

try:
    _encoding = QtGui.QApplication.UnicodeUTF8
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig, _encoding)
except AttributeError:
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig)

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName(_fromUtf8("MainWindow"))
        MainWindow.resize(400, 200)
        self.centralwidget = QtGui.QWidget(MainWindow)
        self.centralwidget.setObjectName(_fromUtf8("centralwidget"))
        self.lcd_num = QtGui.QLCDNumber(self.centralwidget)
        self.lcd_num.setGeometry(QtCore.QRect(10, 30, 311, 60))
        self.lcd_num.setNumDigits(7)
        self.lcd_num.setObjectName(_fromUtf8("lcd_num"))
        self.lbl_unit = QtGui.QLabel(self.centralwidget)
        self.lbl_unit.setGeometry(QtCore.QRect(340, 32, 46, 51))
        font = QtGui.QFont()
        font.setPointSize(48)
        self.lbl_unit.setFont(font)
        self.lbl_unit.setAlignment(QtCore.Qt.AlignCenter)
        self.lbl_unit.setObjectName(_fromUtf8("lbl_unit"))
        self.btn_amp = QtGui.QPushButton(self.centralwidget)
        self.btn_amp.setGeometry(QtCore.QRect(190, 120, 131, 51))
        self.btn_amp.setObjectName(_fromUtf8("btn_amp"))
        self.btn_volt = QtGui.QPushButton(self.centralwidget)
        self.btn_volt.setGeometry(QtCore.QRect(10, 120, 131, 51))
        self.btn_volt.setObjectName(_fromUtf8("btn_volt"))
        MainWindow.setCentralWidget(self.centralwidget)

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(_translate("MainWindow", "Multímetre", None))
        self.lbl_unit.setText(_translate("MainWindow", "V", None))
        self.btn_amp.setText(_translate("MainWindow", "Amperímetre", None))
        self.btn_volt.setText(_translate("MainWindow", "Voltímetre", None))

