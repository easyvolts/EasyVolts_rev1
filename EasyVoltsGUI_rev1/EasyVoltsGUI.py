# -*- coding: utf-8 -*- 

###########################################################################
## Python code generated with wxFormBuilder (version Mar 29 2017)
## http://www.wxformbuilder.org/
##
## PLEASE DO "NOT" EDIT THIS FILE!
###########################################################################

import wx
import wx.xrc

###########################################################################
## Class MainFrame
###########################################################################

class MainFrame ( wx.Frame ):
	
	def __init__( self, parent ):
		wx.Frame.__init__ ( self, parent, id = wx.ID_ANY, title = u"EasyVoltsGUI_rev1         (by Valerii Proskurin)", pos = wx.DefaultPosition, size = wx.Size( 488,315 ), style = wx.DEFAULT_FRAME_STYLE|wx.TAB_TRAVERSAL )
		
		self.SetSizeHintsSz( wx.Size( 488,315 ), wx.Size( 488,316 ) )
		
		fgSizer1 = wx.FlexGridSizer( 0, 3, 1, 1 )
		fgSizer1.SetFlexibleDirection( wx.BOTH )
		fgSizer1.SetNonFlexibleGrowMode( wx.FLEX_GROWMODE_NONE )
		
		gbSizer15 = wx.GridBagSizer( 0, 0 )
		gbSizer15.SetFlexibleDirection( wx.BOTH )
		gbSizer15.SetNonFlexibleGrowMode( wx.FLEX_GROWMODE_SPECIFIED )
		
		gbSizer15.SetMinSize( wx.Size( 240,300 ) ) 
		self.m_staticText9 = wx.StaticText( self, wx.ID_ANY, u"EasyVolts ports setup", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText9.Wrap( -1 )
		self.m_staticText9.SetFont( wx.Font( 10, wx.FONTFAMILY_DEFAULT, wx.FONTSTYLE_NORMAL, wx.FONTWEIGHT_BOLD, False, wx.EmptyString ) )
		
		gbSizer15.Add( self.m_staticText9, wx.GBPosition( 0, 0 ), wx.GBSpan( 1, 3 ), wx.ALIGN_CENTER_HORIZONTAL|wx.ALL, 2 )
		
		self.m_staticText91 = wx.StaticText( self, wx.ID_ANY, u"Control port:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText91.Wrap( -1 )
		gbSizer15.Add( self.m_staticText91, wx.GBPosition( 1, 0 ), wx.GBSpan( 1, 1 ), wx.ALIGN_CENTER_VERTICAL|wx.ALL, 3 )
		
		m_choice1_comPortChoices = []
		self.m_choice1_comPort = wx.Choice( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, m_choice1_comPortChoices, 0 )
		self.m_choice1_comPort.SetSelection( 0 )
		gbSizer15.Add( self.m_choice1_comPort, wx.GBPosition( 1, 1 ), wx.GBSpan( 1, 2 ), wx.ALIGN_CENTER_VERTICAL|wx.ALIGN_LEFT, 3 )
		
		self.m_staticText111 = wx.StaticText( self, wx.ID_ANY, u"Connection:", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText111.Wrap( -1 )
		gbSizer15.Add( self.m_staticText111, wx.GBPosition( 2, 0 ), wx.GBSpan( 1, 1 ), wx.ALL, 3 )
		
		self.m_staticText121_connectStatus = wx.StaticText( self, wx.ID_ANY, u"NOT CONNECTED", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText121_connectStatus.Wrap( -1 )
		self.m_staticText121_connectStatus.SetFont( wx.Font( wx.NORMAL_FONT.GetPointSize(), wx.FONTFAMILY_DEFAULT, wx.FONTSTYLE_NORMAL, wx.FONTWEIGHT_BOLD, False, wx.EmptyString ) )
		self.m_staticText121_connectStatus.SetForegroundColour( wx.Colour( 255, 0, 0 ) )
		
		gbSizer15.Add( self.m_staticText121_connectStatus, wx.GBPosition( 2, 1 ), wx.GBSpan( 1, 2 ), wx.ALIGN_CENTER_VERTICAL|wx.ALIGN_LEFT, 3 )
		
		
		fgSizer1.Add( gbSizer15, 1, wx.ALIGN_LEFT|wx.FIXED_MINSIZE, 0 )
		
		self.m_staticline1 = wx.StaticLine( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, wx.LI_VERTICAL )
		fgSizer1.Add( self.m_staticline1, 0, wx.ALL|wx.EXPAND, 5 )
		
		gbSizer16 = wx.GridBagSizer( 0, 0 )
		gbSizer16.SetFlexibleDirection( wx.BOTH )
		gbSizer16.SetNonFlexibleGrowMode( wx.FLEX_GROWMODE_SPECIFIED )
		
		gbSizer16.SetMinSize( wx.Size( 230,300 ) ) 
		self.m_staticText10 = wx.StaticText( self, wx.ID_ANY, u"Output", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText10.Wrap( -1 )
		self.m_staticText10.SetFont( wx.Font( 10, wx.FONTFAMILY_DEFAULT, wx.FONTSTYLE_NORMAL, wx.FONTWEIGHT_BOLD, False, wx.EmptyString ) )
		
		gbSizer16.Add( self.m_staticText10, wx.GBPosition( 0, 1 ), wx.GBSpan( 1, 1 ), wx.ALIGN_CENTER_HORIZONTAL|wx.ALL, 5 )
		
		self.m_staticText15 = wx.StaticText( self, wx.ID_ANY, u"power control", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText15.Wrap( -1 )
		self.m_staticText15.SetFont( wx.Font( wx.NORMAL_FONT.GetPointSize(), wx.FONTFAMILY_DEFAULT, wx.FONTSTYLE_NORMAL, wx.FONTWEIGHT_BOLD, False, wx.EmptyString ) )
		
		gbSizer16.Add( self.m_staticText15, wx.GBPosition( 1, 1 ), wx.GBSpan( 1, 1 ), wx.ALIGN_CENTER_HORIZONTAL|wx.ALL, 2 )
		
		self.m_staticText11 = wx.StaticText( self, wx.ID_ANY, u" voltage", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText11.Wrap( -1 )
		gbSizer16.Add( self.m_staticText11, wx.GBPosition( 2, 0 ), wx.GBSpan( 1, 1 ), wx.ALL, 5 )
		
		self.m_staticText12 = wx.StaticText( self, wx.ID_ANY, u"current limit", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText12.Wrap( -1 )
		gbSizer16.Add( self.m_staticText12, wx.GBPosition( 2, 2 ), wx.GBSpan( 1, 1 ), wx.ALL, 5 )
		
		self.m_slider1_voltage = wx.Slider( self, wx.ID_ANY, 0, 0, 15000, wx.DefaultPosition, wx.DefaultSize, wx.SL_INVERSE|wx.SL_VERTICAL )
		self.m_slider1_voltage.SetMinSize( wx.Size( -1,145 ) )
		
		gbSizer16.Add( self.m_slider1_voltage, wx.GBPosition( 3, 0 ), wx.GBSpan( 5, 1 ), wx.ALIGN_CENTER_HORIZONTAL|wx.ALL, 2 )
		
		self.m_slider2_current = wx.Slider( self, wx.ID_ANY, 1000, 0, 1000, wx.DefaultPosition, wx.DefaultSize, wx.SL_INVERSE|wx.SL_VERTICAL )
		self.m_slider2_current.SetMinSize( wx.Size( -1,145 ) )
		
		gbSizer16.Add( self.m_slider2_current, wx.GBPosition( 3, 2 ), wx.GBSpan( 5, 1 ), wx.ALIGN_CENTER_HORIZONTAL|wx.ALL, 2 )
		
		self.m_staticText13_actualVoltage = wx.StaticText( self, wx.ID_ANY, u"0.00 V", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText13_actualVoltage.Wrap( -1 )
		self.m_staticText13_actualVoltage.SetFont( wx.Font( 14, wx.FONTFAMILY_DEFAULT, wx.FONTSTYLE_NORMAL, wx.FONTWEIGHT_BOLD, False, wx.EmptyString ) )
		
		gbSizer16.Add( self.m_staticText13_actualVoltage, wx.GBPosition( 3, 1 ), wx.GBSpan( 1, 1 ), wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_TOP|wx.ALL, 5 )
		
		self.m_staticText15_targetVoltage = wx.StaticText( self, wx.ID_ANY, u"0 v", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText15_targetVoltage.Wrap( -1 )
		self.m_staticText15_targetVoltage.SetFont( wx.Font( 12, wx.FONTFAMILY_DEFAULT, wx.FONTSTYLE_ITALIC, wx.FONTWEIGHT_NORMAL, False, wx.EmptyString ) )
		
		gbSizer16.Add( self.m_staticText15_targetVoltage, wx.GBPosition( 4, 1 ), wx.GBSpan( 1, 1 ), wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_TOP|wx.BOTTOM, 5 )
		
		self.m_checkBox1_autoSetVoltage = wx.CheckBox( self, wx.ID_ANY, u"Set values automatically", wx.DefaultPosition, wx.DefaultSize, 0 )
		gbSizer16.Add( self.m_checkBox1_autoSetVoltage, wx.GBPosition( 8, 0 ), wx.GBSpan( 1, 3 ), wx.ALL, 5 )
		
		self.m_toggleBtn1 = wx.ToggleButton( self, wx.ID_ANY, u"Apply values", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_toggleBtn1.SetValue( True ) 
		gbSizer16.Add( self.m_toggleBtn1, wx.GBPosition( 9, 1 ), wx.GBSpan( 1, 1 ), wx.ALIGN_CENTER_HORIZONTAL|wx.ALL, 2 )
		
		self.m_staticText14_actualCurrent = wx.StaticText( self, wx.ID_ANY, u"0.00 A", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText14_actualCurrent.Wrap( -1 )
		self.m_staticText14_actualCurrent.SetFont( wx.Font( 14, wx.FONTFAMILY_DEFAULT, wx.FONTSTYLE_NORMAL, wx.FONTWEIGHT_BOLD, False, wx.EmptyString ) )
		
		gbSizer16.Add( self.m_staticText14_actualCurrent, wx.GBPosition( 5, 1 ), wx.GBSpan( 1, 1 ), wx.ALIGN_CENTER_HORIZONTAL|wx.TOP, 5 )
		
		self.m_staticText16_targetCurrent = wx.StaticText( self, wx.ID_ANY, u"1 a", wx.DefaultPosition, wx.DefaultSize, 0 )
		self.m_staticText16_targetCurrent.Wrap( -1 )
		self.m_staticText16_targetCurrent.SetFont( wx.Font( 12, wx.FONTFAMILY_DEFAULT, wx.FONTSTYLE_ITALIC, wx.FONTWEIGHT_NORMAL, False, wx.EmptyString ) )
		
		gbSizer16.Add( self.m_staticText16_targetCurrent, wx.GBPosition( 6, 1 ), wx.GBSpan( 1, 1 ), wx.ALIGN_CENTER|wx.ALL, 5 )
		
		
		fgSizer1.Add( gbSizer16, 1, wx.EXPAND, 5 )
		
		
		self.SetSizer( fgSizer1 )
		self.Layout()
		self.m_timer1 = wx.Timer()
		self.m_timer1.SetOwner( self, wx.ID_ANY )
		self.m_timer1.Start( 150 )
		
		
		self.Centre( wx.BOTH )
		
		# Connect Events
		self.Bind( wx.EVT_CLOSE, self.closeProgramHandler )
		self.m_choice1_comPort.Bind( wx.EVT_CHOICE, self.controlPortSelectionHandler )
		self.m_slider1_voltage.Bind( wx.EVT_SCROLL, self.voltageScrollChanged )
		self.m_slider2_current.Bind( wx.EVT_SCROLL, self.currentScrollChanged )
		self.m_toggleBtn1.Bind( wx.EVT_TOGGLEBUTTON, self.applyValuesButtonToggle )
		self.Bind( wx.EVT_TIMER, self.guiTickTimer150ms, id=wx.ID_ANY )
	
	def __del__( self ):
		pass
	
	
	# Virtual event handlers, overide them in your derived class
	def closeProgramHandler( self, event ):
		event.Skip()
	
	def controlPortSelectionHandler( self, event ):
		event.Skip()
	
	def voltageScrollChanged( self, event ):
		event.Skip()
	
	def currentScrollChanged( self, event ):
		event.Skip()
	
	def applyValuesButtonToggle( self, event ):
		event.Skip()
	
	def guiTickTimer150ms( self, event ):
		event.Skip()
	

