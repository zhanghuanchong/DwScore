<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="JuryCenter.aspx.cs" Inherits="WashHands.Web.JuryCenter" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <title>评分中心</title>
    <link href="Style/jury.css" rel="stylesheet" type="text/css" />
    <script type="text/javascript" src="Script/jquery-1.3.2.min.js"></script>
    <script type="text/javascript" src="Script/jury.js"></script>
    <script type="text/javascript" src="Script/jquery.maskedinput-1.2.2.min.js"></script>
    <script type="text/javascript" src="Script/common.js"></script>
</head>
<body>
    <form id="form1" runat="server" enableviewstate="False" onkeydown="return checkIfEnter(event.keyCode);">
    <div id="juryCenter">
        <div id="systemName" runat="server"></div>
        <div id="moduleName" runat="server"></div>
        <div id="welcome" runat="server"></div>
        <div id="tip">您无需并且最好不要刷新页面（You need't refresh this page and please don't.） </div>
        <div id="status"><div id="statusLunci" runat="server"></div><div id="message"></div></div>
        <div id="jurydiv" runat="server">
            <table id="jurybox" runat="server" cellspacing="0"></table>
            <input id="juryID" type="hidden" runat="server" />
            <input id="lunciID" type="hidden" runat="server" value="0" />
            <input id="changciID" type="hidden" runat="server" value="0"/>
        </div>
        <div id="host" runat="server"></div>
        <div id="logoutbox">
            <asp:ImageButton ID="logout" runat="server" Height="18px" 
                ImageUrl="~/Style/images/close.gif" onclick="logout_Click" ToolTip="安全退出" 
                Width="27px" />
        </div>
    </div>
    </form>
</body>
</html>
