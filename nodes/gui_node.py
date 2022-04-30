#!/usr/bin/env python3

import sys
import rospy

# Qt -------------------------------
from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import (QWidget, QLCDNumber, QSlider,
                             QVBoxLayout, QLabel, QPushButton,
                             QHBoxLayout, QApplication)
from std_msgs.msg import Int32, Float32MultiArray, Empty, Int32MultiArray


class UIClass(QWidget):
    def __init__(self):  # Konstrukor
        # Konstruktor der Elternklasse aufrufen
        super(UIClass, self).__init__()

        rospy.init_node('setpoint_node', anonymous=True)

        self.setpoint_pub = rospy.Publisher('/setpoint',Int32, queue_size=10)
        #self.pids_pub = rospy.Publisher('/pids',Float32MultiArray, queue_size=10)
        self.pids_pub = rospy.Publisher('/pids',Int32MultiArray, queue_size=10)
        self.ref_pub = rospy.Publisher('toggle_led', Empty, queue_size=10)

        rospy.Subscriber('/adc', Int32, self.postion_cb)

        self.setpoint = 0
        self.position = 0
        self.kp = 0
        self.ki = 0
        self.kd = 0

        self.initUI()

    def initUI(self):
        # setpoint
        self.setpoint_label = QLabel('setpoint')
        self.setpoint_lcd = QLCDNumber(self)
        self.setpoint_lcd.display(self.setpoint)
        self.setpoint_slider = QSlider(Qt.Horizontal, self)
        self.setpoint_slider.setMaximum(700)
        self.setpoint_slider.setMinimum(0)
        self.setpoint_slider.setValue(self.setpoint)

        # position
        self.position_label = QLabel('position')
        self.position_lcd = QLCDNumber(self)
        self.position_lcd.display(self.position)

        setpoint_layout = QVBoxLayout()
        setpoint_layout2 = QHBoxLayout()
        setpoint_layout2.addWidget(self.setpoint_label)
        setpoint_layout2.addWidget(self.setpoint_lcd)
        setpoint_layout2.addWidget(self.position_label)
        setpoint_layout2.addWidget(self.position_lcd)
        setpoint_layout.addLayout(setpoint_layout2)
        setpoint_layout.addWidget(self.setpoint_slider)
        self.setpoint_slider.valueChanged.connect(self.setpoint_slot)  # publish to ROS

        # kp
        self.kp_label = QLabel('kp')
        self.kp_lcd = QLCDNumber(self)
        self.kp_lcd.display(self.kp)
        self.kp_slider = QSlider(Qt.Horizontal, self)
        self.kp_slider.setMaximum(100) # 1 * 100
        self.kp_slider.setMinimum(0)
        self.kp_slider.setValue(self.kp)
        kp_layout = QHBoxLayout()
        kp_layout.addWidget(self.kp_label)
        kp_layout.addWidget(self.kp_slider)
        kp_layout.addWidget(self.kp_lcd)
        self.kp_slider.valueChanged.connect(self.pids_slot)

        # ki
        self.ki_label = QLabel('ki')
        self.ki_lcd = QLCDNumber(self)
        self.ki_lcd.display(self.ki)
        self.ki_slider = QSlider(Qt.Horizontal, self)
        self.ki_slider.setMaximum(100)
        self.ki_slider.setMinimum(0)
        self.ki_slider.setValue(self.ki)
        ki_layout = QHBoxLayout()
        ki_layout.addWidget(self.ki_label)
        ki_layout.addWidget(self.ki_slider)
        ki_layout.addWidget(self.ki_lcd)
        self.ki_slider.valueChanged.connect(self.pids_slot) 

        # kd
        self.kd_label = QLabel('kd')
        self.kd_lcd = QLCDNumber(self)
        self.kd_lcd.display(self.kd)
        self.kd_slider = QSlider(Qt.Horizontal, self)
        self.kd_slider.setMaximum(100)
        self.kd_slider.setMinimum(0)
        self.kd_slider.setValue(self.kd)
        kd_layout = QHBoxLayout()
        kd_layout.addWidget(self.kd_label)
        kd_layout.addWidget(self.kd_slider)
        kd_layout.addWidget(self.kd_lcd)
        self.kd_slider.valueChanged.connect(self.pids_slot) 

        # reference
        self.ref_button = QPushButton(' reference ')
        self.ref_button.clicked.connect(self.ref_slot)

        # layout
        layout = QVBoxLayout()
        layout.addLayout(setpoint_layout)
        layout.addLayout(kp_layout)
        layout.addLayout(ki_layout)
        layout.addLayout(kd_layout)
        layout.addWidget(self.ref_button)
        self.setLayout(layout)

        # window
        self.setGeometry(300, 300, 250, 200)
        self.setWindowTitle('Lagerregler')
        self.show()

    def setpoint_slot(self):
        self.setpoint_lcd.display(self.setpoint_slider.value())
        self.setpoint_pub.publish(int(self.setpoint_lcd.value()))
        
    def pids_slot(self):
        #self.kp_lcd.display(float(self.kp_slider.value()/100.0))
        #self.ki_lcd.display(float(self.ki_slider.value()/100.0))
        #self.kd_lcd.display(float(self.kd_slider.value()/100.0))

        self.kp_lcd.display(float(self.kp_slider.value()/100.0))
        self.ki_lcd.display(float(self.ki_slider.value()/100.0))
        self.kd_lcd.display(float(self.kd_slider.value()/100.0))
        
        array = [int(self.kp_slider.value()), int(self.ki_slider.value()), int(self.kd_slider.value())]
        #array_to_pub = Float32MultiArray(data=array)
        array_to_pub = Int32MultiArray(data=array)
        self.pids_pub.publish(array_to_pub)

    def ref_slot(self):
        self.ref_pub.publish()

    def postion_cb(self, data):
        self.position_lcd.display(data.data)
        self.position_label.update # repaint show

if __name__ == '__main__':
    try:
        #print(sys.argv)
        app = QApplication(sys.argv)
        ui = UIClass()
        sys.exit(app.exec_())
        

    except rospy.ROSInterruptException:
        pass
