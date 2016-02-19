$(document).ready(function() {
	var result;
	console.log("test");
	   jQuery.get( "https://public-api.wordpress.com/rest/v1.1/sites/crichtonobservatoryblog.wordpress.com/posts/", function( response ) { 
	    // response contains site information
	    //console.log(response.posts[0]);
	    for (var i = 0; i < response.found; i++) {
		var post = response.posts[i];
		result = "<div class='post'>";
		result += "<b>" + post.title + "</b>" + post.content + post.date;
jQuery.get( "https://public-api.wordpress.com/rest/v1.1/sites/crichtonobservatoryblog.wordpress.com/posts/" + post.ID + "/replies/", function( response ) { 
	console.log(response); 
	for (var j = 0; j < response.found; j++) {
		var comment = response.comments[j];
		console.log(response.comments[j]);	
		result += "<br/>" + comment.content + "<br/>" + comment.date; 
		console.log(result);
	}
});
		result += "</div>";
console.log(result);
		$('#content').append(result);
		}
	} ); 
});


