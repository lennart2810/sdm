#!/usr/bin/env python3

import sys
import rospy

# Qt -------------------------------
from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import (QWidget, QLCDNumber, QSlider,
                             QVBoxLayout,
                             QHBoxLayout, QApplication)
from std_msgs.msg import Int32


class UIClass(QWidget):
    def __init__(self):  # Konstrukor
        # Konstruktor der Elternklasse aufrufen
        super(UIClass, self).__init__()

        rospy.init_node('setpoint_node', anonymous=True)

        self.setpoint_pub = rospy.Publisher('/setpoint',Int32, queue_size=10)

        self.initUI()

        # self.rate = rospy.Rate(1)
        # while not rospy.is_shutdown():
        #     #self.setpoint_pub.publish(self.sldX.value())
        #     #self.rate.sleep()


    def initUI(self):
        # Instanziierung der Widgets
        LCDstartWert = 0
        self.lcdX = QLCDNumber(self)
        self.lcdX.display(LCDstartWert)

        self.sldX = QSlider(Qt.Horizontal, self)
        self.sldX.setMaximum(700)
        self.sldX.setMinimum(0)
        self.sldX.setValue(LCDstartWert)

        # BOX-Layout mit Widgets füllen
        vbox = QVBoxLayout()
        #  0.Reihe - Label
        hbox = QHBoxLayout()
        #hbox.addWidget(self.lblInfoX)
        vbox.addLayout(hbox)
        # 1.Reihe
        hbox = QHBoxLayout()
        hbox.addWidget(self.lcdX)
        vbox.addLayout(hbox)
        # 2.Reihe
        hbox = QHBoxLayout()
        hbox.addWidget(self.sldX)
        vbox.addLayout(hbox)

        # Alle Boxen ins Window setzen
        self.setLayout(vbox)

        # Fenster Konfigurieren
        self.setGeometry(300, 300, 500, 150)
        self.setWindowTitle('Cart - Setpoint (ADC-Value)')
        self.show()

        # Signal und Slot verbinden
        self.sldX.valueChanged.connect(self.lcdX.display)
        self.sldX.valueChanged.connect(self.SlotPublish)  # publish to ROS

    def SlotPublish(self):
        self.setpoint_pub.publish(self.sldX.value())
 

if __name__ == '__main__':
    try:
        print(sys.argv)
        app = QApplication(sys.argv)
        ui = UIClass()
        sys.exit(app.exec_())
        

    except rospy.ROSInterruptException:
        pass
