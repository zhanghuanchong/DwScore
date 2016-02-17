<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="AdminCenter.aspx.cs" Inherits="WashHands.Web.AdminCenter" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <title></title>
    <link href="Style/admin.css" rel="stylesheet" type="text/css" />
    <link href="Style/jquery-ui-1.7.2.custom.css" rel="stylesheet" type="text/css" />
    <script type="text/javascript" src="Script/jquery-1.3.2.min.js"></script>
    <script type="text/javascript" src="Script/common.js"></script>
    <script type="text/javascript" src="Script/jquery-ui-1.7.2.custom.min.js"></script>
    <script type="text/javascript" src="Script/admin.js"></script>
</head>
<body>
    <form id="form1" runat="server" enableviewstate="False">
    <input type="hidden" id="currentLunciID" runat="server" />
    <div id="adminCenter">
        <div id="systemName" runat="server"></div>
        <div id="moduleName" runat="server"></div>
        <div id="message"></div>
        <div id="tabs">
	        <ul>
		        <li><a href="#tabs-1">系统初始化</a></li>
		        <li><a href="#tabs-2">前台展示管理</a></li>
		        <li><a href="#tabs-3">晋级选手初始化</a></li>
	        </ul>
	        <div id="tabs-1">
	            <h3><a href="#" onclick="accordion(1)">第一步：输入系统信息</a></h3>
	            <div>
	                <table cellspacing="0" id="sysinfo">
                        <tr>
                            <td>比赛名称：</td>
                            <td>中文：<input id="sName" name="sName" type="text" />　英文：<input id="esName" name="esName" type="text" /></td>
                        </tr>
                        <tr>
                            <td>管理员密码：</td>
                            <td>新　：<input id="adminPwd" name="adminPwd" type="password" />　确认：<input id="adminPwd2" name="adminPwd2" type="password" /><span id="adminPwdTip" class="tip">不修改请留空</span></td>
                        </tr>
                        <tr>
                            <td>前台展示密码：</td>
                            <td>新　：<input id="showPwd" name="showPwd" type="password" />　确认：<input id="showPwd2" name="showPwd2" type="password" /><span id="showPwdTip" class="tip">不修改请留空</span></td>
                        </tr>
                        <tr>
                            <td>主办单位：</td>
                            <td>中文：<input id="oName" name="oName" type="text" />　英文：<input id="eoName" name="eoName" type="text" /></td>
                        </tr>
                        <tr>
                            <td>承办单位：</td>
                            <td>中文：<input id="cName" name="cName" type="text" />　英文：<input id="ecName" name="ecName" type="text" /></td>
                        </tr>
                        <tr>
                            <td colspan="2" style="text-align:center"><input class="button" id="systemInit" type="button" value="提交" onclick="systemInit_click()"/></td>
                        </tr>
                    </table>
                    <p style="display:none" id="modifyOK">修改成功！</p>
                </div>
                <h3><a href="#" onclick="accordion(2)">第二步：输入评委信息</a></h3>
                <div>
                    <table cellspacing="0"></table>
                    <div class="add" onclick="addInfo()">+添加</div>
                    <div class="fieldset" id='fs2'>
                        <fieldset>
                            <label>　　　ID：<input type="text" id="2" disabled="disabled"/></label>
                            <label>　　姓名：<input type="text" id="20"/></label>
                            <label>　　职称：<input type="text" id="21"/></label>
                            <label>　　单位：<input type="text" id="22"/></label>
                            <label>研究领域：<input type="text" id="23"/></label>
                            <label>　登录名：<input type="text" id="24"/>*唯一</label>
                            <label>登录密码：<input type="text" id="25"/></label>
                        </fieldset>
                    </div>
                </div>
                <h3><a href="#" onclick="accordion(3)">第三步：输入轮次信息</a></h3>
                <div>
                    <table cellspacing="0"></table>
                    <div class="add" onclick="addInfo()">+添加</div>
                    <div class="fieldset" id='fs3'>
                        <fieldset>
                            <label>　　　　ID：<input type="text" id="3" disabled="disabled"/></label>
                            <label>　　　名称：<input type="text" id="30"/></label>
                            <label>　　英文名：<input type="text" id="31"/></label>
                            <label>　　场次数：<input type="text" id="32"/></label>
                            <label>晋级选手数：<input type="text" id="33"/></label>
                        </fieldset>
                    </div>
                </div>
                <h3><a href="#" onclick="accordion(4)">第四步：输入环节信息</a></h3>
                <div>
                    <table cellspacing="0"></table>
                    <div class="add" onclick="addInfo()">+添加</div>
                    <div class="fieldset" id='fs4'>
                        <fieldset>
                            <label>　　　　　　ID：<input type="text" id="4" disabled="disabled"/></label>
                            <label>　　　　　名称：<input type="text" id="40"/></label>
                            <label>　　　　英文名：<input type="text" id="41"/></label>
                            <label>　　　隶属轮次：<input type="text" id="42"/></label>
                            <label>　　　比赛时间：<input type="text" id="43"/></label>
                            <label>第一次响铃时间：<input type="text" id="44"/></label>
                            <label>第二次响铃时间：<input type="text" id="45"/></label>
                            <label>　　　最短时间：<input type="text" id="46"/></label>
                        </fieldset>
                    </div>
                </div>
                <h3><a href="#" onclick="accordion(5)">第五步：输入选手信息</a></h3>
                <div>
                    <table cellspacing="0"></table>
                    <div class="add" onclick="addInfo()">+添加</div>
                    <div class="fieldset" id='fs5'>
                        <fieldset>
                            <label>　　　ID：<input type="text" id="5" disabled="disabled"/></label>
                            <label>　　姓名：<input type="text" id="51"/></label>
                            <label>演讲题目：<input type="text" id="52"/></label>
                            <label>　　单位：<input type="text" id="53"/></label>
                            <label>　　场次：<input type="text" id="54"/></label>
                            <label>　　序号：<input type="text" id="55"/></label>
                        </fieldset>
                    </div>
                </div>
                <h3><a href="#" onclick="accordion(6)">第六步：初始化</a></h3>
                <div>
                    <div style="font-size:small;text-align:left; text-indent:2em">点击初始化按钮，该操作将删除所有成绩及排名信息，并将系统重置到初始状态。之后您就可以在“前台展示”部分进行控制了。</div>
                    <input class="button" type="button" id="bInit" value="初始化" onclick="initSystem()"/>
                    <p style="display:none" id="initTip">初始化成功！</p>
                </div>
	        </div>
	        <div id="tabs-2">
	            <fieldset>
	                <legend>计时器管理</legend>
                    <input type="button" class="button" id="begintiming" name="begintiming" value="开始计时" runat="server" onclick="beginTiming();" />
                    <input type="button" class="button" id="timezero" name="timezero" value="归零" onclick="timeZero()" />
                    <input type="button" class="button" id="timeend" name="timeend" value="停止" onclick="timeEnd()" />
                </fieldset>
                <fieldset>
                    <legend>排名管理</legend>
                    <input type="button" class="button" id="showrank" name="showrank" value="显示排名" onclick="showRank()" />
                </fieldset>
                <ol style="text-align:left;font-size:small">
                    <li>当该轮次最后一个选手比赛完毕后，请点击“显示排名”按钮（必需）；</li>
                    <li>之后请重新输入晋级选手的场次号和序号；</li>
                    <li>点击“开始计时”按钮开始新一轮次的比赛。</li>
                </ol>
	        </div>
	        <div id="tabs-3">
		        <table cellspacing="0"></table>
		        <input class="button" type="button" value="提交" id="ubutton" onclick="uSubmit()" style="display:none" />
	        </div>
        </div>
        <div id="host" runat="server"></div>
    </div>
    </form>
</body>
</html>
