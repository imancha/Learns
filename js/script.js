var xmlhttp;
function GetXmlHttpObject(){
	if(window.XMLHttpRequest){return new XMLHttpRequest();}	// code for IE7+, Firefox, Chrome, Opera, Safari
	if(window.ActiveXObject){return new ActiveXObject("Microsoft.XMLHTTP");}	// code for IE6, IE5
	return null;
}
function login(a,b){
	xmlhttp = GetXmlHttpObject();
	xmlhttp.onreadystatechange = function() {
		if(xmlhttp.readyState == 4 && xmlhttp.status == 200) {
//			document.getElementById("content").innerHTML = xmlhttp.responseText;
		}
	}
	xmlhttp.open("GET","./ajax/ajax.php?kelas="+a+"&pengajar="+b,true);
	xmlhttp.send();
}
