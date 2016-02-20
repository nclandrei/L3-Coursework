$(document).ready(function($){

  $(".newspage-btnCommentPost").click(function() {
    var clicked = $(this).data().clicked;
    if (clicked == "0") {
      $(this).parent().parent().find(".newspage-containerNewComment").css("display", "block");
      $(this).parent().parent().find(".newspage-containerComment").css("display", "block");
      $(this).data("clicked", "1");
    } else {
      $(this).parent().parent().find(".newspage-containerNewComment").css("display", "none");
      $(this).parent().parent().find(".newspage-containerComment").css("display", "none");
      $(this).data("clicked", "0");
    }
  });
  
  $(".glossarypage-containerH1").click(function() {
    var clicked = $(this).data().clicked;
    if (clicked == "0") {
      $(this).parent().parent().find(".glossarypage-containerText").css("display", "block");
      $(this).data("clicked", "1");
    } else {
      $(this).parent().parent().find(".glossarypage-containerText").css("display", "none");
      $(this).data("clicked", "0");
    }
  });


});
