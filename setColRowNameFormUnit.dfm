object setCollRowNameForm: TsetCollRowNameForm
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biHelp]
  BorderStyle = bsDialog
  Caption = #1047#1072#1076#1072#1081#1090#1077' '#1080#1084#1103
  ClientHeight = 130
  ClientWidth = 282
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  Scaled = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 96
    Top = 16
    Width = 93
    Height = 13
    Caption = #1042#1074#1077#1076#1080#1090#1077' '#1085#1072#1079#1074#1072#1085#1080#1077
  end
  object Edit1: TEdit
    Left = 64
    Top = 35
    Width = 161
    Height = 21
    TabOrder = 0
  end
  object Button1: TButton
    Left = 64
    Top = 80
    Width = 75
    Height = 25
    Caption = 'Ok'
    Default = True
    TabOrder = 1
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 150
    Top = 80
    Width = 75
    Height = 25
    Cancel = True
    Caption = #1054#1090#1084#1077#1085#1072
    TabOrder = 2
    OnClick = Button2Click
  end
end
