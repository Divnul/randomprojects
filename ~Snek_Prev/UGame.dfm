object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Form1'
  ClientHeight = 645
  ClientWidth = 839
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  OnKeyDown = FormKeyDown
  PixelsPerInch = 96
  TextHeight = 13
  object PB: TPaintBox
    Left = 8
    Top = 29
    Width = 816
    Height = 608
    OnPaint = PBPaint
  end
  object Image1: TImage
    Left = 840
    Top = 512
    Width = 105
    Height = 105
  end
  object lblshow: TLabel
    Left = 640
    Top = 8
    Width = 31
    Height = 13
    Caption = 'Fruits:'
  end
  object tim: TTimer
    Interval = 100
    OnTimer = timTimer
    Left = 176
    Top = 32
  end
end
