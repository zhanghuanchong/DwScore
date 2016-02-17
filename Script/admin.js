var lid;
var url;
var index;
var os;
var uStr;

var tHeadArray = ["<tr class='tableHead'><td>ID</td><td>姓名</td><td>职称</td><td>单位</td><td>研究领域</td><td>登录名</td></tr>",
    "<tr class='tableHead'><td>ID</td><td>名称</td><td>英文名</td><td>场次数</td><td>晋级选手数</td></tr>",
    "<tr class='tableHead'><td>ID</td><td>名称</td><td>英文名</td><td>隶属轮次</td><td>比赛时间</td><td>第一次响铃时间</td><td>第二次响铃时间</td><td>最短时间</td></tr>",
    "<tr class='tableHead'><td>ID</td><td>姓名</td><td>演讲题目</td><td>单位</td><td>场次</td><td>序号</td></tr>"];
var wArray = [350,340,360,350];
var hArray = [280,240,300,260];

function accordion(i, u) {
    index = i;
    $("#tabs-1 h3 + div").hide(500);
    var oDiv = $("#tabs-1 h3 + div:eq(" + (index - 1) + ")");
    if (oDiv.css("display") == "none" || u == "u") {
        oDiv.show(500);
        $.get("Handlers/GetInitInfo.ashx?time=" + new Date().getTime().toString() + "&step=" + index, function(data) {
            var str = data.toString();
            var f = parseInt(str.charAt(0));
            var r;
            //系统信息
            if (f == 1) {
                r = str.split("|");
                $("#sName").val(unescape(r[1])); $("#esName").val(unescape(r[2]));
                $("#oName").val(unescape(r[3])); $("#eoName").val(unescape(r[4]));
                $("#cName").val(unescape(r[5])); $("#ecName").val(unescape(r[6]));
                //评委信息、轮次信息、环节信息、选手信息
            } else if (f >= 2 && f <= 5) {
                r = str.split("|");
                var t = $("#tabs-1 h3 + div:eq(" + (index-1) + ") table");
                t.empty();
                var appendStr = tHeadArray[index - 2];
                for (var i = 1; i < r.length; i++) {
                    var json = eval("(" + r[i] + ")");
                    appendStr += "<tr onclick='modifyDialog(this)' title='" + json.i + "' style='cursor:pointer' onmouseover='this.style.backgroundColor=\"#e5ecf9\"' onmouseout='this.style.backgroundColor=\"white\"'>";
                    for (var js in json) {
                        appendStr += "<td>" + json[js] + "</td>";
                    }
                    appendStr += "</tr>";
                }
                t.append(appendStr);
                initDialog();
                //初始化
            } else if (f == 6) {
            }
        });
    }
}

function wReplace(str) {
    return str.replace("'", "’").replace("\"", "＂").replace(",", "，").replace("|", "！").replace(" ", "　");
}

function initDialog(w, h) {
    $("#fs" + index).dialog({
        bgiframe: true,
        autoOpen: false,
        width: wArray[index-2],
        height: hArray[index-2],
        modal: true
    }).dialog("option", "buttons", {
        删除: function() {
            if (confirm("您确定要删除该记录吗？")) {
                $.get("Handlers/SubmitInitInfo.ashx?time=" + new Date().getTime().toString() + "&step=" + index + "&action=del&id=" + $("#fs" + index).attr("title"), function() { alert("删除成功"); $("#fs" + index).dialog('close'); accordion(index); accordion(index, "u"); });
            }
        },
        修改: function() {
            url = "Handlers/SubmitInitInfo.ashx?time=" + new Date().getTime().toString() + "&step=" + index + "&action=modify&id=" + $("#fs" + index).attr("title") + "&data=";
            $("#fs" + index + " input").each(function(i, d) { url += escape(wReplace($.trim($(d).val()))) + "|" });
            $.get(url, function() {
                alert("修改成功"); $("#fs" + index).dialog('close'); accordion(index); accordion(index, "u");
            });
        }
    });
}

function modifyDialog(dom) {
    os = $(dom).children();
    os.each(function(idx, obj) { $("#fs" + index + " input:eq(" + idx + ")").val($(obj).html().replace("&nbsp;", "")); });
    $("#fs" + index).attr("title", $(dom).attr("title"));
    $("#fs" + index).dialog('open');
}

function addInfo() {
    $("#fs" + index + " input").each(function(i, d) { $(d).val(""); });
    $("#fs" + index).dialog("option", "buttons", {
        添加: function() {
            url = "Handlers/SubmitInitInfo.ashx?time=" + new Date().getTime().toString() + "&step=" + index + "&id=0&action=add&data=";
            $("#fs" + index + " input").each(function(i, d) { url += escape(wReplace($.trim($(d).val()))) + "|" });
            $.get(url, function() {
                alert("添加成功"); $("#fs" + index).dialog('close'); accordion(index); accordion(index, "u");
            });
        }
    }
    ).dialog('open');
}

function systemInit_click() {
    if ($("#adminPwd").val() != $("#adminPwd2").val()) {
        $("#adminPwdTip").text("两次密码不同");
        return false;
    }
    if ($("#showPwd").val() != $("#showPwd2").val()) {
        $("#showPwdTip").text("两次密码不同");
        return false;
    }
    $("#modifyOK").show().dialog({
        autoOpen: false,
        modal: true,
        buttons: {
            确定: function() {
                $(this).dialog('close');
                $("#modifyOK").hide();
            }
        }
    });
    var url = "Handlers/SubmitInitInfo.ashx?time=" + new Date().getTime().toString() + "&step=1&data="
        + escape(wReplace($("#sName").val())) + "|" + escape(wReplace($("#esName").val())) + "|"
        + escape(wReplace($("#oName").val())) + "|" + escape(wReplace($("#eoName").val())) + "|"
        + escape(wReplace($("#cName").val())) + "|" + escape(wReplace($("#ecName").val()));
    if ($.trim($("#adminPwd").val()) != "") {
        url += "|" + escape($("#adminPwd").val()) + "|" + escape($("#showPwd").val());
    }
    $.get(url, function() {
        $("#modifyOK").dialog('open');
    });
    $("#tabs-1 h3 + div:eq(0)").hide(500);
}

function initSystem() {
    $.get("Handlers/InitSystem.ashx?time=" + new Date().getTime().toString() + "&action=init", function() {
        $("#initTip").dialog({
            autoOpen: false,
            modal: true,
            buttons: {
                确定: function() {
                    $(this).dialog('close');
                    $("#initTip").hide();
                }
            }
        }).dialog('open');
    });
}

function uSubmit() {
    var tr = $("#tabs-3 table tr");
    var length = tr.length;
    var obj;
    uStr = "";
    tr.each(function(i, d) {
        if (i >= 1 && i<= length-2) {
            obj = $(d).children();
            uStr += "|" + escape(wReplace(obj.eq(0).html())) + "," + escape(wReplace(obj.eq(3).find("input").val())) + "," + escape(wReplace(obj.eq(4).find("input").val())) + "," + escape(wReplace(obj.eq(5).find("input").val()));
        }
    });
    $.get("Handlers/SubmitUpgradePlayers.ashx?time=" + new Date().getTime().toString() + "&do=submit&data=" + uStr, function(data) {
        if (data.toString() == "ok") {
            alert("提交成功！");
            $("#tabs").tabs('select', 1);
            $("#begintiming").attr("disabled", false);
            $("#showrank").attr("disabled", true);
        }
    });
}

//交互部分
$(function() {
    $("#tabs").tabs({ selected: 1 });
    lid = $("#currentLunciID").val();
    $("#tabs-1 h3 + div").hide();
    $("#showrank").attr("disabled", true);
});

function sendMsg(handler, json, sender) {
    var url="Handlers/"+handler+"?time=" + new Date().getTime().toString() + json;
    $.get(url, function(data) {
        if (data == "started") {
            $(sender).attr("disabled", true);
        }
    });
}

var intv;

function beginTiming() {
    $.get("Handlers/StartTiming.ashx?time=" + new Date().getTime().toString() + "&do=start", function(data) {
        if (data == "started") {
            $("#begintiming").attr("disabled", true);
            intv=setInterval(checkIfEnd, 1000);
        }
    });
}

function washHandsEnd(action) {
    sendMsg("StartTiming.ashx", "&do="+action, "");
    $("#begintiming").attr("disabled", false);
    $.get("Handlers/NextHuanjieText.ashx?time=" + new Date().getTime().toString() + "&lid=" + lid, function(data) {
        var dstr = data.toString();
        var dArray = dstr.split("|");
        if (dArray[0] != lid) {
            $("#begintiming").attr("disabled", true);
            $("#showrank").attr("disabled", false);
            lid = dArray[0];
        }
        $("#begintiming").val(dArray[1] + "开始计时");
    });
}

function timeZero() {
    washHandsEnd("clear");
}

function timeEnd() {
    washHandsEnd("stop");
}

function checkIfEnd() {
    $.get("Handlers/TimeOver.ashx?time=" + new Date().getTime().toString() + "&req=check", function(data) {
        var dstr = data.toString();
        var f = dstr.charAt(0);
        if (f == 'e') {
            clearInterval(intv);
            $("#begintiming").attr("disabled", false);
        }
    });
}

function showRank() {
    $.get("Handlers/DoShowRank.ashx?time=" + new Date().getTime().toString() + "&do=showRank", function() {
        $.get("Handlers/GetUpgradePlayers.ashx?time=" + new Date().getTime().toString(), function(data) {
            var uArray = data.toString().split("|");
            var tu = $("#tabs-3 table");
            tu.empty();
            tu.append("<tr class=\"tableHead\"><td>ID</td><td>姓名</td><td>单位</td><td>演讲题目</td><td>场次</td><td>序号</td></tr>");
            for (var i = 1; i < uArray.length; i++) {
                var json = eval("(" + uArray[i] + ")");
                tu.append("<tr><td>" + json.i + "</td><td>" + json.n + "</td><td>" + json.d + "</td><td><input type='text' /></td><td><input type='text' style='width:50px' value='1' /></td><td><input type='text' style='width:50px' /></td></tr>");
            }
            tu.append("<tr><td colspan='6' style='text-align:center'><input class='button' type='button' value='提交' id='ubutton' onclick='uSubmit()' /></td></tr>");
            $("#tabs").tabs('select', 2);
        });
    });
}
