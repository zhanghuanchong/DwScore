var huanjieCount, no;
var juryID;
var intv;
var lunciID;
var changciID;

$(function() {
    juryID = parseInt($("#juryID").val());
    lunciID = parseInt($("#lunciID").val());
    changciID = parseInt($("#changciID").val());
    getNextPlayer();
});

function getNextPlayer() {
    $.get("Handlers/GetNextPlayer.ashx?time=" + new Date().getTime().toString() + "&juryID=" + juryID + "&lcID=" + lunciID + "&ccID=" + changciID, function(data) {
        if (data.toString().charAt(0) == 'a') {
            $("#message").text("该轮次全部选手已经评分完毕，感谢您的辛勤工作！(This round completed, Thank you!)");
        } else if (data.toString().charAt(0) == 'e') {
            $("#message").text("评分已提交，请等待其他评委的评分！(Submitted, please wait!)");
        } else {
            clearInterval(intv);
            var dstr = data.toString();
            var dstrArray = dstr.split("|");
            var f = dstrArray[0];
            var str = "";
            if (f != "{}") {
                var json = eval("(" + dstrArray[0] + ")");
                lunciID = json.lcid;
                changciID = json.cc;
                $("#statusLunci").html(json.lc + "，第" + changciID + "场（No." + changciID + "）");
                var hjArray = json.hj.split(";");
                $("#jurybox").empty().append("<tr id=\"juryboxhead\"><td>选手(Player)</td><td>题目(Title)</td></tr>");
                for (var i = 0; i < hjArray.length; i++) {
                    $("#juryboxhead").append("<td>" + hjArray[i] + "</td>");
                }
                $("#juryboxhead").append("<td>提交(Submit)</td>");
                if (json.sp != "") {
                    var pArray = json.sp.split(";");
                    for (var i = 0; i < pArray.length; i++) {
                        var json2 = eval("(" + pArray[i] + ")");
                        str += "<tr><td>" + json2.o + "号(" + json2.n + ")</td><td>" + json2.t + "</td>";
                        var sArray = json2.s.split(",");
                        for (var j = 0; j < sArray.length; j++) {
                            str += "<td>" + sArray[j] + "</td>";
                        }
                        str += "<td><span>已提交(Submitted)</span></td></tr>";
                    }
                }
            }
            huanjieCount = parseInt(dstrArray[1]);
            if (huanjieCount > 0) {
                str += "<tr>" + dstrArray[2];
                for (var i = 1; i <= huanjieCount; i++) {
                    str += "<td><input type='text' class='inputscore' id='huanjie" + i + "' name='huanjie" + i + "' maxlength='6' /></td>";
                }
                $("#jurybox").append(str + "<td><input type='button' id='submit' name='submit' value='提交(submit)' onclick='submitScore()'/></td></tr>");
                $("input[type=text],input[type=password]").bind("focus", function() {
                    $(this).css("border", "solid 1px  #000").css("backgroundColor", "#ffa").val("0");
                }).bind("blur", function() {
                    $(this).css("border", "solid 1px  #aaa").css("backgroundColor", "#fff");
                    var v = this.value;
                    if (v.search(/\d{1,3}\.\d{0,2}/) == -1 || parseFloat(v) > 100.00 || parseFloat(v) < 0.00) {
                        $("#message").html("请输入规范的分数(0.00~100.00)！Error score format!").hide().show(1000);
                        $(this).val("");
                    }
                });
                $("#huanjie1").focus();
                no = dstr.substring(dstr.indexOf("<td>") + 4, dstr.indexOf("号"));
                $("#message").text("请为" + no + "号选手评分！(Please score No." + no + " player!)").hide().show(1000);
                $(".inputscore").mask("999.99", { placeholder: " " });
            }
        }
    });
}

function submitScore() {
    var score = "";
    for (var i = 1; i <= huanjieCount; i++) {
        var h = parseFloat($("#huanjie" + i).val());
        if ( isNaN(h) ) {
            $("#message").html("请输入您的评分！Please enter your rating!").hide().show(1000);
            $("#huanjie1").focus();
            return;
        }
        score += "&huanjie"+i+"="+h;
        $("#huanjie" + i).parent().append("<span>" + h + "</span>");
        $("#huanjie" + i).remove();
        $("#submit").parent().append("<span>已提交(Submitted)</span>");
        $("#submit").remove();
        $("#message").html("评分已提交，请等待其他评委的评分！(Submitted, please wait!)").hide().show(1000);
    }
    $.get("Handlers/SubmitScore.ashx?time=" + new Date().getTime().toString() + "&lid=" + lunciID + "&cid=" + changciID + "&juryID=" + juryID + "&playerOrder=" + no + score, function() {
        intv = setInterval(getNextPlayer, 1000);
    });
}

function checkIfEnter(k) {
    if (k == 13) {
        $("#submit").click();
        return false;
    }
}