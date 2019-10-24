var cookie_namespace = 'doxygen'; 
var sidenav,navtree,content,header;

function readCookie(cookie) 
{
  var myCookie = cookie_namespace+"_"+cookie+"=";
  if (document.cookie) 
  {
    var index = document.cookie.indexOf(myCookie);
    if (index != -1) 
    {
      var valStart = index + myCookie.length;
      var valEnd = document.cookie.indexOf(";", valStart);
      if (valEnd == -1) 
      {
        valEnd = document.cookie.length;
      }
      var val = document.cookie.substring(valStart, valEnd);
      return val;
    }
  }
  return 0;
}

function writeCookie(cookie, val, expiration) 
{
  if (val==undefined) return;
  if (expiration == null) 
  {
    var date = new Date();
    date.setTime(date.getTime()+(10*365*24*60*60*1000)); // default expiration is one week
    expiration = date.toGMTString();
  }
  document.cookie = cookie_namespace + "_" + cookie + "=" + val + "; expires=" + expiration+"; path=/";
}
 
function updateWidth()
{
  var contentsWidth = Math.min(Math.max(400,$(window).width()-$(sidenav).outerWidth()-48),960);
  $('div.contents').width(contentsWidth);
  $('div.header').width(contentsWidth);
}

function resizeWidth() 
{
  updateWidth();
  var windowWidth = $(window).width() + "px";
  var sidenavWidth = $(sidenav).outerWidth();
  content.css({marginLeft:parseInt(sidenavWidth)+6+"px"}); //account for 6px-wide handle-bar
  writeCookie('width',sidenavWidth, null);
}

function restoreWidth(navWidth)
{
  updateWidth();
  var windowWidth = $(window).width() + "px";
  content.css({marginLeft:parseInt(navWidth)+6+"px"});
  sidenav.css({width:navWidth + "px"});
}

function resizeHeight() 
{
  updateWidth();
  var headerHeight = header.outerHeight();
  var footerHeight = footer.outerHeight();
  var windowHeight = $(window).height() - headerHeight - footerHeight;
  content.css({height:windowHeight + "px"});
  navtree.css({height:windowHeight + "px"});
  sidenav.css({height:windowHeight + "px",top: headerHeight+"px"});
}

function initResizable()
{
  header  = $("#top");
  sidenav = $("#side-nav");
  content = $("#doc-content");
  navtree = $("#nav-tree");
  footer  = $("#nav-path");
  $(".side-nav-resizable").resizable({resize: function(e, ui) { resizeWidth(); } });
  $(window).resize(function() { resizeHeight(); });
  var width = readCookie('width');
  if (width) { restoreWidth(width); } else { resizeWidth(); }
  resizeHeight();
  var url = location.href;
  var i=url.indexOf("#");
  if (i>=0) window.location.hash=url.substr(i);
  var _preventDefault = function(evt) { evt.preventDefault(); };
  $("#splitbar").bind("dragstart", _preventDefault).bind("selectstart", _preventDefault);
  $(document).bind('touchmove',function(e){
    try {
      var target = e.target;
      while (target) {
        if ($(target).css('-webkit-overflow-scrolling')=='touch') return;
        target = target.parentNode;
      }
      e.preventDefault();
    } catch(err) {
      e.preventDefault();
    }
  });
}


