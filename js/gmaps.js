var geocoder;
var map;
var marker;
function initial(){
	geocoder = new google.maps.Geocoder();
	var latlng = new google.maps.LatLng(-6.9026003, 107.6187131);
	var mapOptions = {
		zoom: 15,
		center: latlng
	}
	map = new google.maps.Map(document.getElementById('map-canvas'), mapOptions);
}
function initialize() {
	geocoder = new google.maps.Geocoder();
//	var latlng = new google.maps.LatLng(-6.9026003, 107.6187131);
	var mapOptions = {
		zoom: 15,
//		center: latlng
	}
	// Try HTML5 geolocation
  if(navigator.geolocation) {
    navigator.geolocation.getCurrentPosition(function(position) {
      var pos = new google.maps.LatLng(position.coords.latitude,
                                       position.coords.longitude);

      var infowindow = new google.maps.InfoWindow({
        map: map,
        position: pos,
        content: 'Sepertinya kamu sekarang berada di sekitar sini...'
      });

      map.setCenter(pos);
    }, function() {
      handleNoGeolocation(true);
    });
  } else {
    // Browser doesn't support Geolocation
    handleNoGeolocation(false);
  }

	map = new google.maps.Map(document.getElementById('map-canvas'), mapOptions);
}
function handleNoGeolocation(errorFlag) {
  if (errorFlag) {
    var content = 'Error: The Geolocation service failed.';
  } else {
    var content = 'Error: Your browser doesn\'t support geolocation.';
  }

  var options = {
    map: map,
    position: new google.maps.LatLng(-6.9026003, 107.6187131),
    content: content
  };

  var infowindow = new google.maps.InfoWindow(options);
  map.setCenter(options.position);
}

function codeAddress() {
	var address = document.getElementById('alamat').value;
	geocoder.geocode( { 'address': address}, function(results, status) {
		if (status == google.maps.GeocoderStatus.OK) {
			map.setCenter(results[0].geometry.location);
				marker = new google.maps.Marker({
				map: map,
				title: address,
				animation: google.maps.Animation.DROP,
				position: results[0].geometry.location
			});
			google.maps.event.addListener(marker, 'click', toggleBounce);
		}
	});
}
function setAddress(){
	marker.setMap(null);
	marker = null;
	codeAddress();
}
function toggleBounce() {
	if (marker.getAnimation() != null) {
		marker.setAnimation(null);
	} else {
		marker.setAnimation(google.maps.Animation.BOUNCE);
	}
}
function init(x){
	if(x == 'x'){
		google.maps.event.addDomListener(window, 'resize', initialize);
		google.maps.event.addDomListener(window, 'load', initialize);
	}else{
		google.maps.event.addDomListener(window, 'resize', initial);
		google.maps.event.addDomListener(window, 'load', initial);
		google.maps.event.addDomListener(window, 'resize', codeAddress);
		google.maps.event.addDomListener(window, 'load', codeAddress);
	}
}
