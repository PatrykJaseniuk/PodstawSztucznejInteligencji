object ImportPeriod: TImportPeriod
  Left = 351
  Top = 289
  BorderStyle = bsToolWindow
  Caption = 'Import okresu'
  ClientHeight = 71
  ClientWidth = 349
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Month: TComboBox
    Left = 12
    Top = 20
    Width = 145
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 0
    Items.Strings = (
      'styczeñ'
      'luty'
      'marzec'
      'kwiecieñ'
      'maj'
      'czerwiec'
      'lipiec'
      'sierpieñ'
      'wrzesieñ'
      'paŸdziernik'
      'listopad'
      'grudzieñ')
  end
  object Edit1: TEdit
    Left = 176
    Top = 20
    Width = 49
    Height = 21
    TabOrder = 1
    Text = '2002'
  end
  object UpDown1: TUpDown
    Left = 225
    Top = 20
    Width = 16
    Height = 21
    Associate = Edit1
    Min = 1990
    Max = 2100
    Position = 2002
    TabOrder = 2
    Thousands = False
    Wrap = False
  end
  object Button1: TButton
    Left = 264
    Top = 8
    Width = 75
    Height = 25
    Caption = '&Wstaw'
    TabOrder = 3
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 264
    Top = 40
    Width = 75
    Height = 25
    Caption = '&Zamknij'
    ModalResult = 1
    TabOrder = 4
  end
end
