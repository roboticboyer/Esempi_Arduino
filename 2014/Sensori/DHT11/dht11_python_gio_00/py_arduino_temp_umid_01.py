#!/usr/bin/env python

import sys
from PyQt4 import QtGui, QtCore, uic
import serial
import time

#---Form Principale----------------------------------------------------
class Principale(QtGui.QMainWindow):
	def __init__(self):
		QtGui.QMainWindow.__init__(self)
		self.ui =uic.loadUi('py_arduino_temp_umid_00.ui', self) #Carica form.
		#Posizione e dimensione della finestra.
		self.ui.setGeometry(200, 200, 800, 800)#pos O,V dim O,V
		self.connect(self.ui.B_quit, QtCore.SIGNAL("clicked()"),fine_sessione)
		self.connect(self.ui.B_con, QtCore.SIGNAL("clicked()"),Connessione_on)
		self.connect(self.ui.B_con_2, QtCore.SIGNAL("clicked()"),Connessione_off)
		self.connect(self.ui.B_legge, QtCore.SIGNAL("clicked()"),Legge_Arduino)

#---Legge da Arduino-----------------------------------------
def Legge_Arduino():
	#file_name="dati_arduino.txt"
	#file_out=open(file_name, "w")
	window_a.ui.T_note.setText("Ricezione in corso")
	app.processEvents()
	window_a.ui.update
	i=1
	dato_ok=0
	if conn.isOpen() == True:
		conn.write("L")	 #Richiesta dati dalla seriale 
		time.sleep(0.5)
		while 1:
			try:
				linea=conn.readline() #Lettura linea
				i=i+1
				print linea
				if len(linea) != 0:
					if linea[0] == "D":
						window_a.ui.T_terminale.append(linea)
						app.processEvents()
						linea_ok=linea.rstrip()
						dato_ok=1
					if linea[0] == "E":
						#conn.close()
						window_a.ui.T_note.setText("Lettura avvenuta")
						break
				if i==20:
					window_a.ui.T_note.setText("Connessione Time out")
					break
			
			except serial.serialutil.SerialException:
				print "Errore di comunicazione"
				break
	else:
		window_a.ui.T_note.setText("ATTIVA LA CONNESSIONE")
	app.processEvents()
	if dato_ok == 1:
		#dato_str=''.join(linea_ok)
		#file_out.write(dato_str +"\n")
		dati_dato_str=linea_ok.split(';')
		umidita= dati_dato_str[1]
		temperatura=dati_dato_str[2]
		#print umidita
		window_a.ui.lcdNumber.display(temperatura)
		window_a.ui.lcdNumber_2.display(umidita)
		#window_a.ui.textEdit.append("File salvato con successo")
		#file_out.close()

#---Attiva Connessione---------------------------------------
def Connessione_on():
	# Apre connesione seriale con Arduino
	global conn
	try:
		A_ser_port="/dev/ttyACM0"
		A_ser_vel="9600"
		conn=serial.Serial(A_ser_port, A_ser_vel,timeout=0.5)
		window_a.ui.T_note.setText("Connessione con Arduino avvenuta")
		window_a.ui.B_legge.show()
		window_a.ui.B_con_2.show()
		window_a.ui.B_con.hide()
	except serial.serialutil.SerialException:
		#print "Controllare le connessioni con Arduino"
		window_a.ui.T_note.setText("Controllare le connessioni con Arduino")
	#for i in range(1,2):
		#linea=conn.readline()
	time.sleep(2)
	
#---Chiude connessione---------------------------------------
def Connessione_off():
	conn.close()
	window_a.ui.T_note.setText("Connessione Off")
	window_a.ui.B_con.show()
	window_a.ui.B_con_2.hide()
	window_a.ui.B_legge.hide()
#---Fine lavoro, chiude tutto------------------------------------------
def fine_sessione():

	quit()
#---Funzione principale------------------------------------------------
if __name__ == '__main__':
	app = QtGui.QApplication(sys.argv)
	window_a = Principale()
	window_a.show()
	window_a.ui.B_con_2.hide()
	window_a.ui.B_legge.hide()
	sys.exit(app.exec_())
