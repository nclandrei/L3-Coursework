var margin = {top: 20, right: 20, bottom: 30, left: 40},
    width = 960 - margin.left - margin.right,
    height = 700 - margin.top - margin.bottom;

// data gathered can be changed (the ones present here are taken from actual users)

var data = [[0,1393,1,"exp"], [0.929,966,1,"PI"], [0.772,1584,0,"/"], [1.465,2231,1,"*"], [0.275,2816,1,"3"], [0.772,983,1,")"], [1.891,983,1,"-"], [1.465,1605,1,"log2"],
            [0.929,880,0,"sin"], [0.276,1728,1,"cos"], [0.276,2280,1,"3"], [1.465,1467,1,"2"], [0.772,972,1,")"], [2.170,723,1,")"], [1.432,2440,0,"*"],
            [0.332,1001,1,"/"], [1.509,613,1,"sqrt"], [1.623,720,1,"tan"], [2.564,653,0,"4"], [0.448,972,1,"3"], [0.448,968,1,"*"], [1.263,1205,1,"PI"],
			[1.386,1001,1,")"], [0.332,613,1,")"], [1.740,720,1,"+"], [1.509,653,1,"1"], [1.947,972,1,"9"], [1.623,968,1,"="]];

// setup x
var xValue = function(d) { return d[0];}, // data -> value
    xScale = d3.scale.linear().range([0, width]), // value -> display
    xMap = function(d) { return xScale(xValue(d));}, // data -> display
    xAxis = d3.svg.axis().scale(xScale).orient("bottom");

// setup y
var yValue = function(d) { return d[1];}, // data -> value
    yScale = d3.scale.linear().range([height, 0]), // value -> display
    yMap = function(d) { return yScale(yValue(d));}, // data -> display
    yAxis = d3.svg.axis().scale(yScale).orient("left");

// setup fill color
var cValue = function(d) { return d[3];},
    color = d3.scale.category10(),
    color1 = ["Error", "Success"];

// add the graph canvas to the body of the webpage
var svg = d3.select("body").append("svg")
    .attr("width", width + margin.left + margin.right)
    .attr("height", height + margin.top + margin.bottom)
  .append("g")
    .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

// add the tooltip area to the webpage
var tooltip = d3.select("body").append("div")
    .attr("class", "tooltip")
    .style("opacity", 0);

// load data

  svg.append("text")
        .attr("x", (width / 2))             
        .attr("y", 3)
        .attr("text-anchor", "middle")  
        .style("font-size", "16px")  
	  .text("User 3 : exp(PI*3)-log2(cos(32))/sqrt(tan(3*PI))+19");

  data.forEach(function(d) {
      d[0] = +d[0];
      d[1] = +d[1];

      xScale.domain([d3.min(data, xValue), d3.max(data, xValue) + 1]);
      yScale.domain([d3.min(data, yValue), d3.max(data, yValue) + 100]);

      // x-axis
      svg.append("g")
              .attr("class", "x axis")
              .attr("transform", "translate(0," + height + ")")
              .call(xAxis)
              .append("text")
              .attr("class", "label")
              .attr("x", width)
              .attr("y", -6)
              .style("text-anchor", "end")
              .text("Index of Difficulty");

      // y-axis
      svg.append("g")
              .attr("class", "y axis")
              .call(yAxis)
              .append("text")
              .attr("class", "label")
              .attr("transform", "rotate(-90)")
              .attr("y", 6)
              .attr("dy", ".71em")
              .style("text-anchor", "end")
              .text("Time taken (milliseconds)");

      // draw dots
      svg.selectAll(".dot")
              .data(data)
              .enter().append("circle")
              .attr("class", "dot")
              .attr("r", 5)
              .attr("cx", xMap)
              .attr("cy", yMap)
              .style("fill", function (d) {
                  if (d[2] == 1) return "Green"; else return "Red";
              })
              .on("mouseover", function (d) {
                  tooltip.transition()
                          .duration(200)
                          .style("opacity", .9);
                  tooltip.html(color1[d[2]] + "<br/> (" + xValue(d)
                          + ", " + yValue(d) + ")" + "<br/> Pressed: " + d[3])
                          .style("left", (d3.event.pageX + 5) + "px")
                          .style("top", (d3.event.pageY - 28) + "px");
              })
              .on("mouseout", function (d) {
                  tooltip.transition()
                          .duration(500)
                          .style("opacity", 0);
              });
  });