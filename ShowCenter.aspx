<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="ShowCenter.aspx.cs" Inherits="WashHands.Web.ShowCenter" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <title>展示中心</title>
    <link href="Style/show.css" rel="stylesheet" type="text/css" />
    <script src="Script/jquery-1.3.2.min.js" type="text/javascript"></script>
    <script src="Script/show.js" type="text/javascript"></script>
</head>
<body>
    <form id="form1" runat="server" enableviewstate="False">
    <div id="showCenter">
        <div id="systemName" runat="server"></div>
        <div id="LunciName" runat="server"></div>
        <div id="statusdiv"><div id="status"></div><span id="message"></span></div>
        <div id="timer">
            <div id="currentHuanjie">　</div>
            <table cellspacing="0">
              <tr>
                <td align="right"><strong>　已 用：</strong></td>
                <td><span id="HourText1"></span><span id="MinText1">00</span><span>:</span><span id="SecText1">00</span></td>
                <td align="left"><strong>(Time spent)</strong></td>
              </tr>
              <tr>
                <td align="right"><strong>　剩 余：</strong></td>
                <td><span id="HourText2"></span><span id="MinText2">00</span><span>:</span><span id="SecText2">00</span></td>
                <td align="left"><strong>(Time　left)</strong></td>
              </tr>
            </table>
            <div id="regulation">欢迎各位选手参加本次比赛。比赛分预赛和决赛两部分，预赛由定题演讲和回答问题组成。选手进行3分钟定题演讲，然后回答1名专家提的两个问题，选手回答每个问题的时间各为1分钟。决赛包括即兴演讲和回答问题。选手上场前15分钟抽题。即兴演讲时间为2分钟，然后1名专家就选手即兴演讲的内容提一个问题，选手回答问题时间为1分钟。祝大家赛出风格、赛出成绩、赛出友谊！”
            </div>
        </div>
        <div id="scoreShower"></div>
        <object classid="clsid:22D6F312-B0F6-11D0-94AB-0080C74C7E95" id="MediaPlayer1" style="display:none;"><param name="AnimationAtStart" value="0"/><param name="FileName" value="Media/alert.wav"/><param name="ShowControls" value="-1"/><param name="AutoStart" value="0"/></object>
        <object classid="clsid:22D6F312-B0F6-11D0-94AB-0080C74C7E95" id="MediaPlayer2" style="display:none"><param name="AnimationAtStart" value="0"/><param name="FileName" value="Media/ringin.wav"/><param name="ShowControls" value="-1"/><param name="AutoStart" value="0"/></object>
    </div>
    <div id="host" class="hostfirst" runat="server"></div>
    <input type="hidden" id="currentOrderID" runat="server"/>
    </form>
</body>
</html>
