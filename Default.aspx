<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="Default.aspx.cs" Inherits="WashHands.Web._Default" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <title>评分系统(Scoring System)</title>
    <link href="Style/login.css" rel="stylesheet" type="text/css" />
    <script type="text/javascript" src="Script/jquery-1.3.2.min.js"></script>
    <script type="text/javascript" src="Script/common.js"></script>
    <script type="text/javascript">
    <!--
        $(function(){
            $("#loginbox").hide().slideDown("slow",function(){
                $("#iUserName").focus();
            });
        });
        
        function checkLogin(){
            if($("#iUserName").val()==""){
                $("#errorInfo").text("用户名不能为空!(User name can't be empty!)").show(1000);
                return false;
            }
            if($("#iPassword").val()==""){
                $("#errorInfo").text("密码不能为空!(Password can't be empty!)").show(1000);
                return false;
            }
        }
    //-->
    </script>
</head>
<body>
    <form id="form1" runat="server" onsubmit="return checkLogin();" enableviewstate="False">
    <div id="loginbox">
        <div id="systemName" runat="server">评分系统(Scoring System)</div>
        <ul>
            <li>用户名(User Name)：<input type="text" name="iUserName" id="iUserName" maxlength="10" /></li>
            <li>密　码(Passwrod)&nbsp;&nbsp;：<input type="password" name="iPassword" id="iPassword" maxlength="10" /></li>
            <li>类　型(User Type)&nbsp;：&nbsp;
                <asp:DropDownList ID="ddlUserType" runat="server">
                    <asp:ListItem Selected="True" Value="jury">评委(Jury)</asp:ListItem>
                    <asp:ListItem Value="admin">管理员(Administarte)</asp:ListItem>
                    <asp:ListItem Value="show">前台展示(Show)</asp:ListItem>
                </asp:DropDownList>
            </li>
            <li><asp:Button ID="bSubmit" runat="server" onclick="bSubmit_Click" Text="登录(Log In)" />
            </li>
            <li style="color:Red;font-weight:bold;display:none" id="errorInfo"></li>
        </ul>
    </div>
    <div id="host" runat="server"></div>
    </form>
</body>
</html>
