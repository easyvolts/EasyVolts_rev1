import os

#importing wx files
import wx
 
#import the newly created GUI file
import EasyVoltsGUI
import serial.tools.list_ports
import serial
import configparser
from scanf import scanf
from time import sleep

def getConfig():
    myConfig = configparser.RawConfigParser()
    try:
        myConfig.read("easyvolts.conf")
        if not myConfig.has_section("port"):
            myConfig.add_section("port")
            myConfig.set("port", "name", " ")
        if not myConfig.has_section("autoset_value"):
            myConfig.add_section("autoset_value")
            myConfig.set("autoset_value", "status", "0")  
    except Exception as e:
            print('error: '+ str(e))   
    return myConfig

def saveConfig(myConfig):
    myConfig.write(open("easyvolts.conf", "w"))

def serialOpen(port) :
    ser = serial.Serial(port)
    ser.baudrate=115200*4
    ser.parity=serial.PARITY_NONE
    ser.stopbits=serial.STOPBITS_ONE
    ser.bytesize=serial.EIGHTBITS
    ser.timeout=0.1
    ser.xonxoff=False
    ser.rtscts=False
    ser.dsrdtr=False
    ser.flushInput() #flush input buffer, discarding all its contents
    return ser

def setVoltageCurrent(port, voltage_mV, current_mA) :
    msgByteArray = b'i'+bytes(str(current_mA),'utf-8') + b'\r'
    print(str(msgByteArray))
    port.write(msgByteArray)
    msgByteArray = b'u'+ bytes(str(voltage_mV),'utf-8') + b'\r'
    print(str(msgByteArray))
    port.write(msgByteArray)
    
#inherit from the MainFrame created in wxFowmBuilder and create CalcFrame
class MainFrame(EasyVoltsGUI.MainFrame):
    targetVoltage_mV = 0
    targetCurrent_mA = 1000
    serialPort = 0
    config = 0
    zPullModeTx = 0 #0-noPullUp, 1-pullUp, 2-pullDown
    zPullModeRx = 0 #0-noPullUp, 1-pullUp, 2-pullDown
    
    #constructor
    def __init__(self,parent):
        #initialize parent class       
        EasyVoltsGUI.MainFrame.__init__(self,parent)
        try:
            #get config from file
            self.config = getConfig()
            #scan for available COM ports
            ports = serial.tools.list_ports.comports()
            print('number of ports: ' + str(len(ports)))
            for port in ports:
                  print(port.device + ': ' + port.hwid)                  
            #prepare choiceBox with available ports
            i = 0
            for portData in ports:
                self.m_choice1_comPort.Append(portData.device)
                if(self.config.get("port", "name") == portData.device):
                    self.m_choice1_comPort.SetSelection(i)
                    #generate CHOICE event to do all actions as if user have selected the port
                    wx.PostEvent(self.m_choice1_comPort, wx.CommandEvent(wx.wxEVT_COMMAND_CHOICE_SELECTED )) 
                i = i + 1
            #set checkBox to saved state
            self.m_checkBox1_autoSetVoltage.SetValue(int(self.config.get("autoset_value", "status")))
        except Exception as e:
            print('error: '+ str(e))
 
    #wx calls this function with and 'event' object
    def controlPortSelectionHandler( self, event ):
        try:
            try:
                self.serialPort.close()
            except Exception:
                pass
            print("Open control serial port")
            self.serialPort = serialOpen(self.m_choice1_comPort.GetString(self.m_choice1_comPort.GetSelection()))
            self.m_staticText121_connectStatus.SetLabel('Port OPENED')
            self.m_staticText121_connectStatus.SetForegroundColour( wx.Colour( 255, 255, 0 ) )
            sleep(0.1)
        except Exception as e:
            self.m_staticText121_connectStatus.SetLabel('Can\'t open!')
            self.m_staticText121_connectStatus.SetForegroundColour( wx.Colour( 255, 255, 0 ) )
            print('error: '+ str(e))

    def applyValuesButtonToggle( self, event ):
        try:
            if(self.targetVoltage_mV*self.targetCurrent_mA > 2400000):
                #more than 2Wt power is requested. Reduce current limit to limit power at max 2Wt
                self.targetCurrent_mA = int(2400000 / self.targetVoltage_mV);
                self.m_slider2_current.SetValue(self.targetCurrent_mA)
                self.m_staticText16_targetCurrent.SetLabel(str(self.targetCurrent_mA/1000) + 'a')
            print('Set voltage:' + str(self.targetVoltage_mV) + 'mV. Set current:' + str(self.targetCurrent_mA) + 'mA.')
            setVoltageCurrent(self.serialPort, self.targetVoltage_mV, self.targetCurrent_mA)
            self.m_toggleBtn1.SetValue(False)
        except Exception as e:
            print('error: '+ str(e))

    def voltageScrollChanged( self, event ):
        try:
            self.targetVoltage_mV = self.m_slider1_voltage.GetValue()
            self.m_staticText15_targetVoltage.SetLabel(str(self.targetVoltage_mV/1000) + 'v')
            if(self.m_checkBox1_autoSetVoltage.IsChecked()):
                if(self.targetVoltage_mV*self.targetCurrent_mA > 2400000):
                    #more than 2Wt power is requested. Reduce current limit to limit power at max 2Wt
                    self.targetCurrent_mA = int(2400000 / self.targetVoltage_mV);
                    self.m_slider2_current.SetValue(self.targetCurrent_mA)
                    self.m_staticText16_targetCurrent.SetLabel(str(self.targetCurrent_mA/1000) + 'a')
                setVoltageCurrent(self.serialPort, self.targetVoltage_mV, self.targetCurrent_mA)
                self.m_toggleBtn1.SetValue(False)
            else:
                self.m_toggleBtn1.SetValue(True)
        except Exception as e:
            print('error: '+ str(e))
    
    def currentScrollChanged( self, event ):
        try:
            self.targetCurrent_mA = self.m_slider2_current.GetValue()
            self.m_staticText16_targetCurrent.SetLabel(str(self.targetCurrent_mA/1000) + 'a')
            if(self.m_checkBox1_autoSetVoltage.IsChecked()):
                if(self.targetVoltage_mV*self.targetCurrent_mA > 2400000):
                    #more than 2Wt power is requested. Reduce current limit to limit power at max 2Wt
                    self.targetCurrent_mA = int(2400000 / self.targetVoltage_mV);
                    self.m_slider2_current.SetValue(self.targetCurrent_mA)
                    self.m_staticText16_targetCurrent.SetLabel(str(self.targetCurrent_mA/1000) + 'a')
                setVoltageCurrent(self.serialPort, self.targetVoltage_mV, self.targetCurrent_mA)
                self.m_toggleBtn1.SetValue(False)
            else:
                self.m_toggleBtn1.SetValue(True)
        except Exception as e:
            print('error: '+ str(e))

    def guiTickTimer150ms( self, event ):
        try:
            line = str(self.serialPort.readline(), 'utf-8')
            if(len(line) > 10):
                #we recieve following line: "EasyVolts 	U(mV)=51    	^I(mA)=0". Parse it.
                parsedValues = scanf("EasyVolts 	U(mV)=%d    	%cI(mA)=%d", line)
                if(None != parsedValues):
                    self.m_staticText121_connectStatus.SetForegroundColour( wx.Colour( 0, 100, 0 ) )
                    self.m_staticText13_actualVoltage.SetLabel('{:.02f}'.format(int(parsedValues[0])/1000) + 'V')
                    self.m_staticText14_actualCurrent.SetLabel('{:.03f}'.format(int(parsedValues[2])/1000) + 'A')
                    if(parsedValues[1] == '_'):
                        self.m_staticText13_actualVoltage.SetForegroundColour( wx.Colour( 0, 0, 0 ) )
                        self.m_staticText14_actualCurrent.SetForegroundColour( wx.Colour( 0, 0, 0 ) )
                        self.m_staticText121_connectStatus.SetLabel('CONNECTED, PowerOff')
                    if(parsedValues[1] == ' '):
                        self.m_staticText13_actualVoltage.SetForegroundColour( wx.Colour( 0, 100, 0 ) )
                        self.m_staticText14_actualCurrent.SetForegroundColour( wx.Colour( 0, 100, 0 ) )
                        self.m_staticText121_connectStatus.SetLabel('CONNECTED, PowerOn')
                    if(parsedValues[1] == '^'):
                        self.m_staticText13_actualVoltage.SetForegroundColour( wx.Colour( 255, 255, 0 ) )
                        self.m_staticText14_actualCurrent.SetForegroundColour( wx.Colour( 255, 255, 0 ) )
                        self.m_staticText121_connectStatus.SetLabel('CONNECTED, Overcurrent!')      
        except Exception as e:
            print('error: '+ str(e))
        
    def closeProgramHandler( self, event ):
        try:
            if(self.serialPort != 0):
                self.config.set("port", "name", self.serialPort.port)
                setVoltageCurrent(self.serialPort, 0, 0)
            if(self.m_checkBox1_autoSetVoltage.IsChecked()):
                self.config.set("autoset_value", "status", "1")
            else:
                self.config.set("autoset_value", "status", "0")    
            saveConfig(self.config)
            self.serialPort.close()
            print('exit!')
        except Exception as e:
            print('error: '+ str(e))
        self.Destroy()
	
app = wx.App(False)
frame = MainFrame(None)
frame.Show(True)
app.MainLoop()
