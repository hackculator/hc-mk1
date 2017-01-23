import sys
import serial
from PyQt4 import QtGui, QtCore
from gui import Ui_MainWindow
import threading
import time
import Queue

#Command list
#A Amperes
#V Volts
#E Start
#S Stop

#Communications
class Communications():
	def __init__(self,port, speed):
		self.connected = False
		self.port = port
		self.speed = speed
		
	def connect(self):
		try:
			self.serial = serial.Serial(self.port, timeout=None, baudrate=self.speed)
			self.connected = True
		except:
			raise
		return self.connected

	def disconnect(self):
		try:
			self.serial.close()
			self.connected = False
		except:
			raise
		return self.connected
		
	def send_command(self,cmd):
		try:
			self.serial.write(cmd)
		except:
			raise
	
	def read_line(self):
		try:
			return self.serial.readline()
		except:
			#raise
			pass
		
#Ventana
class Gui(QtGui.QMainWindow):

	def __init__(self):
		super(Gui, self).__init__()
		self.ui=Ui_MainWindow()
		self.ui.setupUi(self)
		self.show()

def signal_amp(ex,com):
	print "Amperes"
	ex.ui.lbl_unit.setText("A")
	com.send_command("A\n")

def signal_volt(ex,com):
	print "Volts"
	ex.ui.lbl_unit.setText("V")
	com.send_command("V\n")

def change_text(ex,com):
	while True:
		value = com.read_line()
		#print valu
		ex.ui.lcd_num.display(str(value)) 
		#time.sleep(0.1)
		
def main():		
	#init
	app = QtGui.QApplication(sys.argv)
	ex = Gui()
	com = Communications("/dev/ttyACM0",9600)
	if( not com.connect()):
		return
	com.send_command("E\n")
	#signal_connection
	ex.ui.btn_amp.clicked.connect(lambda: signal_amp(ex,com))
	ex.ui.btn_volt.clicked.connect(lambda: signal_volt(ex,com))
	ex.ui.lcd_num.setSmallDecimalPoint(True)
	#reading thread
	myThread = threading.Thread(target = change_text,args=[ex,com])
	myThread.daemon = True
	myThread.start()
	#GUI loop
	ret = app.exec_()
	print "bye"
	com.send_command("S\n")
	com.disconnect()
	#closing the window
	sys.exit(ret)
	

if __name__ == '__main__':
    main()
