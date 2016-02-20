$(document).ready(function($){
 $.get( "https://public-api.wordpress.com/rest/v1.1/sites/crichtonobservatoryblog.wordpress.com/posts/", function( response ) {
   for (var i = 0; i < response.found; i++) {
     var result = '<div class="well newspage-containerPost"><div class="row"><h5 class="newspage-headingPost">';
     var post = response.posts[i];
     result += post.title;
     result += '</h5><p class="newspage-datePost">';
     result += post.date;
     result += '</p><div class="newspage-txtPost">';
     result += post.content;
     result += '</div></div><div class="row"><button type="button" class="btn btn-lg btn-default newspage-btnCommentPost" data-clicked="0">0 Comments</button></div>';
     result += '<div class="well newspage-containerNewComment">&nbsp;<div class="row"><h5 class="newspage-lblNewCommentHeader">Add comment</h5></div>';
     result += '<div class="row">' +
         '  <label for="textfield" class="newspage-lblNewCommentName">Name *: </label>'+
         '  <input name="textfield" type="text" class="newspage-txtNewCommentName" id="textfield">'+
     '    </div>'+
       '  <div class="row">'+
         '  <label for="textarea2" class="newspage-lblNewCommentName">Comment*:</label>'+
         '  <textarea name="textarea" class="newspage-txtNewCommentText" id="textarea"></textarea>'+
       '  </div>'+
   '      <div class="row">'+
         '  <button type="button" class="btn btn-sm btn-default newspage-btnNewCommentSubmit">Submit</button>'+
         '</div>'+
   '    </div>'+
   '    <div class="well newspage-containerComment">&nbsp;'+
       '  <div class="row">'+
       '    <h5 class="newspage-lblNewCommentHeader">Comments</h5>'+
       '  </div>'+
       '  <div class="row newspage-containerCommentRow">'+
       '    <h5 class="newspage-txtCommentName">AAAAA BBBBB</h5>'+
       '    <p class="newspage-txtCommentText">Lorem ipsum dolor sit amet, consectetur adipisicing elit. Eos asperiores nihil magni fugit numquam nostrum voluptas cumque vitae ad id!</p>'+
       '  </div>'+
       '  <div class="row newspage-containerCommentRow">'+
       '    <h5 class="newspage-txtCommentName">AAAAA BBBBB</h5>'+
       '    <p class="newspage-txtCommentText">Lorem ipsum dolor sit amet, consectetur adipisicing elit. Similique, itaque, voluptates cumque maxime magni aspernatur sunt optio quo corporis iusto ratione eaque esse explicabo cupiditate assumenda debitis eos doloribus dolores!</p>'+
       '  </div>'+
       '  <div class="row newspage-containerCommentRow">'+
       '    <h5 class="newspage-txtCommentName">AAAAA BBBBB</h5>'+
   '  <p class="newspage-txtCommentText">Lorem ipsum dolor sit amet, consectetur adipisicing elit. Enim, id earum quas dolorum quia consequatur quisquam repellendus facere inventore. Tenetur, magni, praesentium, eius aperiam sint enim tempore officia doloremque ratione sapiente ut laborum quaerat corporis voluptatibus totam facere inventore facilis.</p>'+
       '  </div>' +
     '  </div>' +
     '&nbsp;</div>';
     console.log(result);
     $('.mainpage-container').append(result);
   }
 });

});
