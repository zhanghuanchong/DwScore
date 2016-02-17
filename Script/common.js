$(function() {
    $("input[type=text],input[type=password]").bind("focus", function() {
        $(this).css("border", "solid 1px  #000").css("backgroundColor", "#ffa");
    })
    .bind("blur", function() {
        $(this).css("border", "solid 1px  #aaa").css("backgroundColor", "#fff");
    });
});