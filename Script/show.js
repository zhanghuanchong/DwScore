var intv;
var intvend;
var intvShowRank;
var currentOrder;

//--------------定时器开始
//By Yan Canxun
var flag;
var aa = 0;
var tt = 0;
var tl = 0;
var tw = 0;
var te = 0;
var x = 0;
var y = 0;
var z = 0;
var m = 0;
var n = 0;
var l = 0;
var HourText1;
var MinText1;
var SecText1;
var HourText2;
var MinText2;
var SecText2;
var MediaPlayer1;
var MediaPlayer2;

$(function() {
    HourText1 = document.getElementById("HourText1");
    MinText1 = document.getElementById("MinText1");
    SecText1 = document.getElementById("SecText1");
    HourText2 = document.getElementById("HourText2");
    MinText2 = document.getElementById("MinText2");
    SecText2 = document.getElementById("SecText2");
    MediaPlayer1 = document.getElementById("MediaPlayer1");
    MediaPlayer2 = document.getElementById("MediaPlayer2");
});

//function timelimit(){
//tt=eval(tLimit.value);
//tw=eval(tWarning.value);
//}
function time01() {
    today = new Date();
    aa = today.getTime();
}

function time02() {
    if (aa == 0) {
        time01();
    } else {
        today = new Date();
        bb = today.getTime();
        cc = bb - aa; //cc：已过毫秒数
        te = tl * 60000 - bb + aa; //te：剩余毫秒数
        x = Math.floor(cc / 3600000); //x：已过小时数
        y = Math.floor((cc - x * 3600000) / 60000); //y：已过分钟数
        z = Math.round((cc - x * 3600000 - y * 60000) / 1000); //z：已过秒数
        if (te > 0) {
            m = Math.floor(te / 3600000); //m：剩余小时数
            n = Math.floor((te - m * 3600000) / 60000); //n：剩余分钟数
            l = Math.round((te - m * 3600000 - n * 60000) / 1000); //l：剩余秒数
        } else {
            m = n = l = 0;
        }
        if (x > 0) {
            HourText1.innerText = x + ":";
        }
        if (y == 0) {
            MinText1.innerText = "00";
        }
        if (y > 0) {
            if (y > 9) {
                MinText1.innerText = "" + y;
            }
            else {
                MinText1.innerText = "0" + y;
            }
        }
        if (z > 0) {
            if (z > 9) {
                SecText1.innerText = "" + z;
            }
            else {
                SecText1.innerText = "0" + z;
            }
        } else {
            SecText1.innerText = "00";
        }
        if (m > 0) {
            HourText2.innerText = m + ":";
        }
        else {
            HourText2.innerText = ""
        }
        if (n > 0) {
            if (n > 9) {
                MinText2.innerText = "" + n;
            }
            else {
                MinText2.innerText = "0" + n;
            }
        }
        else {
            MinText2.innerText = "00"
        }
        if (l > 0) {
            if (l > 9) {
                SecText2.innerText = "" + l;
            }
            else {
                SecText2.innerText = "0" + l;
            }
        }
        else {
            SecText2.innerText = "00"
        }
        if (parseInt(cc / 1000) == tt * 60) {
            $("#message").text("　时间到！").hide().show(2000);
            MediaPlayer1.Stop();
            MediaPlayer2.Stop();
            MediaPlayer2.Play();
            clear();
            return;
        }
        if (parseInt(te / 1000) == tw * 60) {
            MediaPlayer1.Stop();
            MediaPlayer2.Stop();
            MediaPlayer1.Play();
        }
    }
    flag = setTimeout("time02()", 1000);
}

function stop() {
    clearTimeout(flag);
    aa = 0;
    $.get("Handlers/TimeOver.ashx?time=" + new Date().getTime().toString() + "&req=end", function(data) {
        var dstr = data.toString();
        if (dstr.charAt(0) == 'e') {
            var dstrArray = dstr.split("|");
            var p = eval("(" + dstrArray[1] + ")");
            currentOrder = p.order;
            $("#status").html("<span>" + p.order + "</span>号选手<span>" + p.name + "</span>比赛完毕，请评委打分！").hide().show(1000);
            $("#message").text("评委打分中！").hide(); $("#currentHuanjie").text("　"); simpleClear();
            intvend = setInterval(getJuryScore, 1000);
        }
    });
}

function simpleClear() {
    tt = 0; tl = 0; tw = 0; HourText1.innerText = ''; MinText1.innerText = '00'; SecText1.innerText = '00'; HourText2.innerText = ''; MinText2.innerText = '00'; SecText2.innerText = '00';
    $("#currentHuanjie").html("　");
    $("#regulation").text("欢迎各位选手参加本次比赛。比赛分预赛和决赛两部分，预赛由定题演讲和回答问题组成。选手进行3分钟定题演讲，然后回答1名专家提的两个问题，选手回答每个问题的时间各为1分钟。决赛包括即兴演讲和回答问题。选手上场前15分钟抽题。即兴演讲时间为2分钟，然后1名专家就选手即兴演讲的内容提一个问题，选手回答问题时间为1分钟。祝大家赛出风格、赛出成绩、赛出友谊！");
}

function clear() {
    stop();
    simpleClear();
}
//-----------定时器完

$(function() {
    currentOrder=parseInt($("#currentOrderID").val());
    intv = setInterval(checkIfTiming, 500);
    if (!Number.prototype.toFixed) {
        Number.prototype.toFixed = function(n) {
            return Math.round(Number(this) * pow(10, n)) / pow(10, n);
        }
    };
});

var last;

function checkIfTiming() {
    $.get("Handlers/StartTiming.ashx?time=" + new Date().getTime().toString() + "&do=check&order=" + currentOrder + "&last=" + last, function(data) {
        var dstr = data.toString();
        if (dstr.charAt(0) == 'w') {
            return;
        }
        clearInterval(intvShowRank);
        if (dstr.charAt(0) == 's' && last != "s") {
            last = "s";
            $("#host").attr("class", "hostend");
            $("#systemName").css("margin-top", "5%");
            $("#LunciName").show(); $("#statusdiv").show(); $("#scoreShower").hide(1000).empty(); $("#timer").show(1000);
            var dstrArray = dstr.split("|");
            var p = eval("(" + dstrArray[2] + ")");
            $("#status").html("<span>" + p.order + "</span>号选手<span>" + p.name + "</span>，来自<span>" + p.danwei + "</span>，参赛题目是：\"<span>" + p.title + "</span>\"");
            $("#message").text("计时中！").hide().show("1000");
            var h = eval("(" + dstrArray[1] + ")");
            clearTimeout(flag); aa = 0; tt = h.secondBellTime; tl = h.standardTime; tw = h.firstBellTime; time02();
            $("#currentHuanjie").html("<strong>当前环节：</strong>" + h.name + "（" + h.ename + "）");
            var myGodTime0 = Math.floor(h.standardTime);
            var myGodTime1 = (h.standardTime - myGodTime0) * 60;
            var myGodTime2 = Math.floor(h.standardTime - h.firstBellTime);
            var myGodTime3 = ((h.standardTime - h.firstBellTime) - myGodTime2) * 60;
            var myGodTime4 = Math.floor(h.secondBellTime);
            var myGodTime5 = (h.secondBellTime - myGodTime4) * 60;
            var myGodStr = myGodTime4 + "分" + myGodTime5 + "秒";
            var myGodTime6 = Math.floor(h.leastTime);
            var myGodTime7 = (h.leastTime - myGodTime6) * 60;
            $("#regulation").text("\"" + h.name + "\"环节时间是" + myGodTime0 + "分" + myGodTime1 + "秒。在" + myGodTime2 + "分" + myGodTime3 + "秒时有一声“叮”声提示，" + myGodStr + "时有一声“叮玲玲”声提示，超过" + myGodStr + "或不足" + myGodTime6 + "分" + myGodTime7 + "秒，扣0.5-1分。" + myGodStr + "时进入超时扣分阶段，屏幕出现“时间到！”。");
            if (dstrArray.length > 3) {
                $("#LunciName").html(dstrArray[3]);
            }
        }
        if (dstr.charAt(0) == 'e' && last != "e" && last != "c" && last != undefined) {
            last = "e";
            $("#message").text("计时停止！").hide().show("1000");
            stop();
        }
        if (dstr.charAt(0) == "c" && last != "c" && last != undefined) {
            $("#message").text("计时器归零！").hide().show("1000");
            if (last == "e") {
                simpleClear();
            } else {
                clear();
            }
            last = "c";
        }
    });
}

var juryNo;
var jScores=new Array();
var hjCount;

function getJuryScore() {
    $.get("Handlers/GetScore.ashx?time=" + new Date().getTime().toString() + "&do=check&order=" + currentOrder, function(data) {
        var dstr = data.toString();
        if (dstr.charAt(0) == "w") {
            return;
        }
        var dstrArray = dstr.split("|");
        hjCount = parseInt(dstrArray[1]);
        var f = dstrArray[0];
        if (f.indexOf('f') != -1) {
            var str = "<table cellspacing='0' cellpadding='0'><tr><td style='padding:0' id='scoreTable'><table cellspacing='0' cellpadding='0'><tr><td>&nbsp;</td></tr>";
            for (var i = 0; i < hjCount; i++) {
                str += "<tr><td>" + dstrArray[2 + i] + "</td></tr>";
                jScores[i] = new Array();
            }
            str += "</table>";
            var juryCount = parseInt(dstrArray[2 + hjCount]);
            juryNo = 0;
            for (var i = 0; i < juryCount; i++) {
                juryNo++;
                str += "<table cellspacing='0' cellpadding='0' id='table" + juryNo + "'><tr><td>评委" + juryNo + "</td></tr>";
                for (var j = 0; j < hjCount; j++) {
                    var dstrScore = dstrArray[3 + hjCount * (i + 1) + j];
                    str += "<tr><td>" + dstrScore + "</td></tr>";
                    jScores[j][juryNo] = parseFloat(dstrScore);
                }
                str += "</table>";
            }
            str += "</td></tr></table>";
            $("#timer").hide(1000);
            $("#scoreShower").append(str).hide().show(1000);
        } else if (f.indexOf('n') != -1) {
            var str = "";
            var juryCount = parseInt(dstrArray[2]);
            for (var i = 0; i < juryCount; i++) {
                juryNo++;
                str += "<table cellspacing='0' cellpadding='0' id='table" + juryNo + "'><tr><td>评委" + juryNo + "</td></tr>";
                for (var j = 0; j < hjCount; j++) {
                    var dstrScore = dstrArray[3 + hjCount * i + j];
                    str += "<tr><td>" + dstrScore + "</td></tr>";
                    jScores[j][juryNo] = parseFloat(dstrScore);
                }
                str += "</table>";
            }
            $("#scoreTable").append(str).hide().show(1000);
        }
        if (f.indexOf('e') != -1) {
            clearInterval(intvend);
            var iSum = 0;
            for (var i = 0; i < hjCount; i++) {
                var iMax = 1; var iMin = 1; iSum += jScores[i][1];
                for (var j = 2; j <= juryNo; j++) {
                    if (jScores[i][iMax] < jScores[i][j])
                        iMax = j;
                    if (jScores[i][iMin] > jScores[i][j])
                        iMin = j;
                    iSum += jScores[i][j];
                }
                $("#table" + iMax + " tr:eq(" + (i + 1) + ")").css("backgroundColor", "yellow");
                $("#table" + iMin + " tr:eq(" + (i + 1) + ")").css("backgroundColor", "blue").css("color", "white");
                iSum -= jScores[i][iMax]; iSum -= jScores[i][iMin];
            }
            var iSumNumber = new Number(iSum/(juryNo-2));
            var iSumStr = iSumNumber.toFixed(2);
            $("#status").html("评委打分完毕！去掉每个环节的最高分(黄色标记),最低分(蓝色标记)，<span>" + currentOrder + "</span>号选手的最终得分：<span style='font-size:55px'>" + iSumStr + "</span>分。").hide().show(1000);
            $("#message").hide(1000);
            $.get("Handlers/ShowCompleted.ashx?time=" + new Date().getTime().toString() + "&do=submit&order=" + currentOrder + "&finalScore=" + iSumStr, function() {
                intvShowRank = setInterval(doShowRank, 1000);
            });
        }
    });
}

function doShowRank() {
    $.get("Handlers/DoShowRank.ashx?time=" + new Date().getTime().toString() + "&do=check", function(data) {
        var dstr = data.toString();
        var f = dstr.charAt(0);
        if (f == 's') {
            clearInterval(intvShowRank);
            var dstrArray = dstr.split("|");
            var count = parseInt(dstrArray[1]);
            var str = "<table style='font-size:xx-large'><tr style='font-weight:bold;'><td>排名</td><td>选手</td><td>姓名</td><td>题目</td><td>总分</td></tr>";
            for (var i = 1; i <= count; i++) {
                var json = eval("(" + dstrArray[i + 1] + ")");
                str += "<tr><td>" + i + "</td><td style='color:red'>第" + json.c + "场第" + json.o + "号</td><td>" + json.n + "</td><td>" + json.t + "</td><td style='color:red'>" + json.s + "</td></tr>";
            }
            str += "</table>";
            $("#timer").hide();
            $("#status").html("<span>本轮次全部选手比赛完毕，晋级选手：</span>").hide().fadeIn(1000);
            $("#scoreShower").empty().append(str).hide().show(1000);
        }
    });
}