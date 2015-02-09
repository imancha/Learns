<?php ob_start(); session_start(); include_once('connect.inc');
	function tanggal($date){
		$Bulan = array("Januari", "Februari", "Maret", "April", "Mei", "Juni", "Juli", "Agustus", "September", "Oktober", "November", "Desember");
		$tahun = substr($date, 0, 4);
		$bulan = substr($date, 5, 2);
		$tgl   = substr($date, 8, 2);
		$result = $tgl . " " . $Bulan[(int)$bulan-1] . " ". $tahun;
		return($result);
	}
	function navigation(){
		if(isset($_GET['daftar'])){$z = ' active';}
		elseif(isset($_GET['masuk'])){$y = ' active';}
		elseif(isset($_GET['profile'])){$c = ' active';}
		elseif(isset($_GET['buatkelas'])){$b = ' active';}
		elseif(isset($_GET['timeline'])){$a = ' active';}
		
		if(isset($_SESSION['imancha']['id'])){
			mysql_open();
			$sql = "SELECT * FROM `User` WHERE `ID`=".$_SESSION['imancha']['id']." LIMIT 1";
			$res = @mysql_query($sql) or die(mysql_error());
			if(mysql_num_rows($res) == 1){
				$row = mysql_fetch_assoc($res);
				$xo .= '<ul class="nav navbar-nav">
									<li class="'.$a.'"><a href="./?timeline">Timeline</a></li>
									<li class="'.$b.'"><a href="./?buatkelas">Buat Kelas</a></li>
									<li class="visible-xs'.$c.'"><a href="./?profile='.$row['Username'].'">Profile</a></li>
									<li class="visible-xs"><a href="./?keluar">Keluar</a></li>
								</ul>
								<ul class="nav navbar-nav navbar-right hidden-xs">
									<li class="dropdown user user-menu">
										<a href="#" class="dropdown-toggle" data-toggle="dropdown">
											<i class="glyphicon glyphicon-user"></i>&nbsp;
											 <span>'.$row['Username'].'&nbsp; <i class="fa fa-caret-down"></i></span>
										</a>
										<ul class="dropdown-menu">
											<li class="user-header">
												<img src="./img/user/'.($row['Avatar']=="" ? "0.png" : $row['Avatar']).'" class="img-circle" alt="User Image" />
												<p>
													'.$row['Nama'].'
													<small>Member sejak - '.tanggal($row['Waktu']).'</small>
												</p>
											</li>
											<li class="user-footer">
												<div class="pull-left">
													<button class="btn btn-info btn-flat" onclick="window.location=\'./?profile='.$row['Username'].'\'">Profile</button>
												</div>
												<div class="pull-right">
													<button class="btn btn-danger btn-flat" onclick="window.location=\'./?keluar\'">Keluar</button>
												</div>
											</li>
										</ul>
									</li>
								</ul>';
			}
			mysql_close();
		}else{
			$xo = '<ul class="nav navbar-nav visible-xs">
							<li class="'.$y.'"><a href="./?masuk">Masuk</a></li>
							<li class="'.$z.'"><a href="./?daftar">Daftar</a></li>
						 </ul>
						 <ul class="nav navbar-nav navbar-right hidden-xs">
							<li class="dropdown">
								<a class="dropdown-toggle" aria-expanded="false" data-close-others="true" data-hover="dropdown" data-toggle="dropdown" href="#"><i class="fa fa-sign-in" style="font-size:larger;"></i></a>
								<ul class="dropdown-menu" role="menu">
									<li><a href="./?masuk">Masuk</a></li>
									<li class="divider"></li>
									<li><a href="./?daftar">Daftar</a></li>
								</ul>
							</li>
						</ul>';
		}
		echo $xo;
	}
	function masuk(){
		if(isset($_POST['submit'])){
			mysql_open();
			$sql = "SELECT * FROM `User` WHERE `Username`='".$_POST['username']."' AND `Password`=SHA1('".$_POST['password']."') LIMIT 1";
			$res = @mysql_query($sql) or die(mysql_error());
			if(mysql_num_rows($res) == 1){
				$row = mysql_fetch_assoc($res);
				session_start();
				$_SESSION['imancha']['id'] = $row['ID'];
				header('Location: ./');
				exit();
			}else{
				$xo .= '<div class="alert alert-danger alert-dismissable" role="alert">
									<button type="button" class="close" data-dismiss="alert" aria-hidden="true">&times;</button>
									<strong>Oops! </strong>
									 Username dan Password tidak cocok. Silahkan coba lagi.
								</div>';
			}
			mysql_close();
		}
		$xo .= '<div class="login">
							<form method="post" action="" role="form">
								<label for="username" class="sr-only">Username or Email</label>
								<input type="text" class="form-control" id="username" name="username" placeholder="Username" value="" autocomplete="off" required autofocus>
								<label for="password" class="sr-only">Password</label>
								<input type="password" class="form-control" id="password" name="password" placeholder="Password" value="" autocomplete="off" required>
								<button class="btn btn-lg btn-block btn-success" type="submit" name="submit">Masuk</button>
							</form>
						 </div>';
		echo $xo;
	}
	function keluar(){
		session_start();
		$_SESSION['imancha'] = array();
		header('Location: ./');
		exit();
	}
	function daftar(){
		if(isset($_POST['submit'])){
			mysql_open();
			mysql_query("START TRANSACTION");
			//	Check username
			$sql = "SELECT `ID` FROM `User` WHERE LOWER(`Username`) LIKE LOWER('".$_POST['username']."') LIMIT 1";
			$res = @mysql_query($sql) or die(mysql_error());
			if(mysql_num_rows($res) == 1){
				$error[] = '<div class="alert alert-warning alert-dismissable" role="alert">
											<button type="button" class="close" data-dismiss="alert" aria-hidden="true">&times;</button>
											<strong>Oops!</strong>
											Username sudah ada yang menggunakan.
										</div>';
			}
			//	Check email
			$sql = "SELECT `ID` FROM `User` WHERE LOWER(`Email`) LIKE LOWER('".$_POST['email']."') LIMIT 1";
			$res = @mysql_query($sql) or die(mysql_error());
			if(mysql_num_rows($res) == 1){
				$error[] = '<div class="alert alert-warning alert-dismissable" role="alert">
											<button type="button" class="close" data-dismiss="alert" aria-hidden="true">&times;</button>
											<strong>Oops! </strong>
											 Email sudah terdaftar.
										</div>';
			}
			if(empty($error)){
				//	Create user
				$sql = "INSERT INTO `User` VALUES (NULL,'".strip_tags(trim(mysql_real_escape_string($_POST['username'])))."',SHA1('".strip_tags(trim(mysql_real_escape_string($_POST['password'])))."'),'".strip_tags(trim(mysql_real_escape_string($_POST['nama'])))."','".strip_tags(trim(mysql_real_escape_string($_POST['email'])))."','".strip_tags(trim(mysql_real_escape_string($_POST['kota'])))."','".strip_tags(trim(mysql_real_escape_string($_POST['phone'])))."',NOW(),'')";
				$res = @mysql_query($sql) or die(mysql_error());
				if(mysql_affected_rows() == 1){
					if($_FILES['avatar']['size'] > 0){
						$id = mysql_insert_id();
						$target_dir = "./img/user/";
						$target_file = $target_dir . basename($_FILES["avatar"]["name"]);
						$uploadOk = 1;
						$imageFileType = pathinfo($target_file,PATHINFO_EXTENSION);
						$newfilename = $id.'.'.$imageFileType;
						$target_file = $target_dir.$newfilename;
						$check = getimagesize($_FILES["avatar"]["tmp_name"]);
						if($check !== false){$uploadOk = 1;}
						else{$uploadOk = 0;
							echo '<div class="alert alert-warning">
											<button class="close" type="button" data-dismiss="alert">x</button>
											<strong>Oops! </strong>
											File bukan berupa gambar.
										</div>';
						}
						if($_FILES["avatar"]["size"] > 500000){$uploadOk = 0;
							echo '<div class="alert alert-warning">
											<button class="close" type="button" data-dismiss="alert">x</button>
											<strong>Oops! </strong>
											Ukuran file terlalu besar.
										</div>';
						}
						if($imageFileType != "png" AND $imageFileType != "jpg" AND $imageFileType != "gif"){$uploadOk = 0;
							echo '<div class="alert alert-warning">
											<button class="close" type="button" data-dismiss="alert">x</button>
											<strong>Oops! </strong>
											File harus berformat .png .jpg .gif
										</div>';
						}
						if($uploadOk == 0){$error = TRUE;}
						else{
							if(!move_uploaded_file($_FILES["avatar"]["tmp_name"], $target_file)){
								$error = TRUE;
								echo '<div class="alert alert-warning">
												<button class="close" type="button" data-dismiss="alert">x</button>
												<strong>Oops! </strong>
												Error upload gambar.
											</div>';
							}else{
								$sql = "UPDATE `User` SET `Avatar`='".$newfilename."' WHERE `ID`=".$id." LIMIT 1";
								$res = @mysql_query($sql) or die(mysql_error());
								if(mysql_affected_rows() != 1){
									echo '<div class="alert alert-warning">
													<button class="close" type="button" data-dismiss="alert">x</button>
													<strong>Oops! </strong>
													Error upload gambar.
												</div>';
								}
							}
						}
					}else{
						$xo .= '<div class="alert alert-success alert-dismissable" role="alert">
											<button type="button" class="close" data-dismiss="alert" aria-hidden="true">&times;</button>
											<strong>Oops! </strong>
											 Gambar kosong.
										</div>';
					}
					mysql_query("COMMIT");
					$xo .= '<div class="alert alert-success alert-dismissable" role="alert">
										<button type="button" class="close" data-dismiss="alert" aria-hidden="true">&times;</button>
										<strong>Selamat! </strong>
										 Kamu telah berhasil terdaftar. Untuk memulai aktifitas pada sistem, silahkan <a href="./?masuk">masuk</a>.
									</div>';
				}else{
					$xo .= '<div class="alert alert-danger alert-dismissable" role="alert">
										<button type="button" class="close" data-dismiss="alert" aria-hidden="true">&times;</button>
										<strong>Oops! </strong>
										 Terjadi kesalahan. Silahkan coba lagi.
									</div>';
				}
			}else{
				mysql_query("ROLLBACK");
				foreach($error as $v){
					$xo .= $v;
				}
			}
			mysql_close();
		}
		$xo .= '<div class="jumbotron">
							<form class="form-horizontal" method="post" action="" enctype="multipart/form-data" role="form">
								<div class="form-group">
									<label for="nama" class="col-xs-12 col-sm-3 col-sm-offset-1 control-label">Nama Lengkap</label>
									<div class="col-xs-12 col-sm-5">
										<input type="text" class="form-control" id="nama" name="nama" placehoder="" value="" autocomplete="off" required autofocus>
									</div>
								</div>
								<div class="form-group">
									<label for="username" class="col-xs-12 col-sm-3 col-sm-offset-1 control-label">Username</label>
									<div class="col-xs-12 col-sm-5">
										<input type="text" class="form-control" id="username" name="username" placeholder="" value="" autocomplete="off" required>
									</div>
								</div>
								<div class="form-group">
									<label for="password" class="col-xs-12 col-sm-3 col-sm-offset-1 control-label">Password</label>
									<div class="col-xs-12 col-sm-5">
										<input type="password" class="form-control" id="password" name="password" placeholder="" value="" autocomplete="off" required>
									</div>
								</div>
								<div class="form-group">
									<label for="email" class="col-xs-12 col-sm-3 col-sm-offset-1 control-label">Email</label>
									<div class="col-xs-12 col-sm-5">
										<input type="email" class="form-control" id="email" name="email" placeholder="" value="" autocomplete="off">
									</div>
								</div>
								<div class="form-group">
									<label for="kota" class="col-xs-12 col-sm-3 col-sm-offset-1 control-label">Kota</label>
									<div class="col-xs-12 col-sm-5">
										<input type="text" class="form-control" id="kota" name="kota" placeholder="" value="" autocomplete="off">
									</div>
								</div>
								<div class="form-group">
									<label for="phone" class="col-xs-12 col-sm-3 col-sm-offset-1 control-label">Phone</label>
									<div class="col-xs-12 col-sm-5">
										<input type="text" class="form-control" id="phone" name="phone" placeholder="" value="" autocomplete="off">
									</div>
								</div>
								<div class="form-group">
									<label for="avatar" class="col-xs-12 col-sm-3 col-sm-offset-1 control-label">Avatar</label>
									<div class="col-xs-12 col-sm-5">
										<div class="fileupload fileupload-new" data-provides="fileupload">
											<div class="input-group">
												<div class="btn btn-file no-pad">
													<button class="btn btn-xs fileupload-new">Pilih Avatar</button>
													<button class="btn btn-xs fileupload-exists">Ganti</button>
													<input type="file" id="avatar" name="avatar">
												</div>
												<button class="btn btn-xs fileupload-exists" data-dismiss="fileupload">Hapus</button>
												<div class="uneditable-input">
													<i class="fileupload-exists"></i>
													<div class="fileupload-preview"></div>
												</div>
											</div>
										</div>
									</div>
								</div>
								<div class="form-group">
									<div class="col-sm-offset-4 col-sm-8">
										<button type="submit" class="btn btn-primary" name="submit">Daftar</button>
										<button type="reset" class="btn btn-default">Reset</button>
									</div>
								</div>
							</form>
						</div>';
		echo $xo;
	}
	function buatkelas(){
		if(!isset($_SESSION['imancha']['id'])){header('Location: ./?masuk'); exit();}
		if(isset($_POST['submit'])){
			mysql_open();
			mysql_query("START TRANSACTION");
			//	Materi
			$sql = "SELECT `ID` FROM `Materi` WHERE LOWER(`Nama`) LIKE LOWER('".$_POST['materi']."') LIMIT 1";
			$res = @mysql_query($sql) or die(mysql_error());
			if(mysql_num_rows($res) == 1){
				$row = mysql_fetch_assoc($res);
				$idm = $row['ID'];
			}else{
				$sql = "INSERT INTO `Materi` VALUES (NULL,'".strip_tags(trim(mysql_real_escape_string($_POST['materi'])))."')";
				$res = @mysql_query($sql) or die(mysql_error());
				if(mysql_affected_rows() == 1){
					$idm = mysql_insert_id();
				}else{
					$error[] = '<div class="alert alert-danger alert-dismissable" role="alert">
												<button type="button" class="close" data-dismiss="alert" aria-hidden="true">&times;</button>
												<strong>[ERR-Materi] </strong>
												 Terjadi kesalahan. Silahkan coba lagi.
											</div>';
				}
			}
			//	Kelas
			$sql = "INSERT INTO `Kelas` VALUES (NULL,'".strip_tags(trim(mysql_real_escape_string($_POST['tanggal'])))."','".strip_tags(trim(mysql_real_escape_string($_POST['jam'])))."','".strip_tags(trim(mysql_real_escape_string($_POST['lokasi'])))."','".strip_tags(trim(mysql_real_escape_string($_POST['alamat'])))."',".strip_tags(trim(mysql_real_escape_string($_POST['kuota']))).",".($_POST['status']=='pelajar' ? '1' : '0').",".$_SESSION['imancha']['id'].",".$idm.")";
			$res = @mysql_query($sql) or die(mysql_error());
			if(mysql_affected_rows() == 1){
				$idk = mysql_insert_id();
				if($_POST['status'] == 'pengajar'){$status = 'Pengajar';}
				else{$status = 'Pelajar';}
				$sql = "INSERT INTO `Belajar` VALUES (".$idk.",".$_SESSION['imancha']['id'].",'".$status."')";
				$res = @mysql_query($sql) or die(mysql_error());
				if(mysql_affected_rows() != 1){
					$error[] = '<div class="alert alert-danger alert-dismissable" role="alert">
												<button type="button" class="close" data-dismiss="alert" aria-hidden="true">&times;</button>
												<strong>[ERR-Belajar] </strong>
												 Terjadi kesalahan. Silahkan coba lagi.
											</div>';
				}
			}else{
				$error[] = '<div class="alert alert-danger alert-dismissable" role="alert">
											<button type="button" class="close" data-dismiss="alert" aria-hidden="true">&times;</button>
											<strong>[ERR-Kelas] </strong>
											 Terjadi kesalahan. Silahkan coba lagi.
										</div>';
			}
			//	Tingkat
			if(!empty($_POST['tingkat'])){
				foreach(explode(',',$_POST['tingkat']) as $value){
					$sql = "SELECT `ID` FROM `Tingkat` WHERE UPPER(`Nama`) LIKE UPPER('".$value."') LIMIT 1";
					$res = @mysql_query($sql) or die(mysql_error());
					if(mysql_num_rows($res) == 1){
						$row = mysql_fetch_assoc($res);
						$idt = $row['ID'];
					}else{
						$sql = "INSERT INTO `Tingkat` VALUES (NULL,UPPER('".strip_tags(trim(mysql_real_escape_string($value)))."'))";
						$res = @mysql_query($sql) or die(mysql_error());
						if(mysql_affected_rows() == 1){
							$idt = mysql_insert_id();
						}else{
							$error[] = '<div class="alert alert-danger alert-dismissable" role="alert">
														<button type="button" class="close" data-dismiss="alert" aria-hidden="true">&times;</button>
														<strong>[ERR-Tingkat] </strong>
														Terjadi kesalahan. Silahkan coba lagi.
													</div>';
						}
					}
					$sql = "INSERT INTO `Tingkat Kelas` VALUES (".$idk.",".$idt.")";
					$res = @mysql_query($sql) or die(mysql_error());
					if(mysql_affected_rows() != 1){
						$error[] = '<div class="alert alert-danger alert-dismissable" role="alert">
													<button type="button" class="close" data-dismiss="alert" aria-hidden="true">&times;</button>
													<strong>[ERR-Tingkat Kelas] </strong>
													Terjadi kesalahan. Silahkan coba lagi.
												</div>';
					}
				}
			}
			//	Jurusan
			if(!empty($_POST['jurusan'])){
				foreach(explode(',',$_POST['jurusan']) as $value){
					$sql = "SELECT `ID` FROM `Jurusan` WHERE LOWER(`Nama`) LIKE LOWER('".$value."') LIMIT 1";
					$res = @mysql_query($sql) or die(mysql_error());
					if(mysql_num_rows($res) == 1){
						$row = mysql_fetch_assoc($res);
						$idj = $row['ID'];
					}else{
						$sql = "INSERT INTO `Jurusan` VALUES (NULL,'".strip_tags(trim(mysql_real_escape_string($value)))."')";
						$res = @mysql_query($sql) or die(mysql_error());
						if(mysql_affected_rows() == 1){
							$idj = mysql_insert_id();
						}else{
							$error[] = '<div class="alert alert-danger alert-dismissable" role="alert">
														<button type="button" class="close" data-dismiss="alert" aria-hidden="true">&times;</button>
														<strong>[ERR-Jurusan] </strong>
														 Terjadi kesalahan. Silahkan coba lagi.
													</div>';
						}
					}
					$sql = "INSERT INTO `Jurusan Kelas` VALUES (".$idk.",".$idj.")";
					$res = @mysql_query($sql) or die(mysql_error());
					if(mysql_affected_rows() != 1){
						$error[] = '<div class="alert alert-danger alert-dismissable" role="alert">
													<button type="button" class="close" data-dismiss="alert" aria-hidden="true">&times;</button>
													<strong>[ERR-Jurusan Kelas] </strong>
													Terjadi kesalahan. Silahkan coba lagi.
												</div>';
					}
				}
			}
			//	Semester
			if(!empty($_POST['kelas'])){
				foreach(explode(',',$_POST['kelas']) as $value){
					$sql = "SELECT `ID` FROM `Semester` WHERE `Nama`=".$value." LIMIT 1";
					$res = @mysql_query($sql) or die(mysql_error());
					if(mysql_num_rows($res) == 1){
						$row = mysql_fetch_assoc($res);
						$ids = $row['ID'];
					}else{
						$sql = "INSERT INTO `Semester` VALUES (NULL,'".strip_tags(trim(mysql_real_escape_string($value)))."')";
						$res = @mysql_query($sql) or die(mysql_error());
						if(mysql_affected_rows() == 1){
							$ids = mysql_insert_id();
						}else{
							$error[] = '<div class="alert alert-danger alert-dismissable" role="alert">
														<button type="button" class="close" data-dismiss="alert" aria-hidden="true">&times;</button>
														<strong>[ERR-Semester] </strong>
														 Terjadi kesalahan. Silahkan coba lagi.
													</div>';
						}
					}
					$sql = "INSERT INTO `Semester Kelas` VALUES (".$idk.",".$ids.")";
					$res = @mysql_query($sql) or die(mysql_error());
					if(mysql_affected_rows() != 1){
						$error[] = '<div class="alert alert-danger alert-dismissable" role="alert">
													<button type="button" class="close" data-dismiss="alert" aria-hidden="true">&times;</button>
													<strong>[ERR-Semester Kelas] </strong>
													Terjadi kesalahan. Silahkan coba lagi.
												</div>';
					}
				}
			}
			if(empty($error)){
				mysql_query("COMMIT");
				$xo .= '<div class="alert alert-success alert-dismissable" role="alert">
									<button type="button" class="close" data-dismiss="alert" aria-hidden="true">&times;</button>
									<strong>Selamat! </strong>
									 Kelas '.$_POST['materi'].' telah berhasil dibuat. <a href="./?kelas='.SHA1($idk).'">Lihat kelas.</a>
								</div>';
			}else{
				mysql_query("ROLLBACK");
				foreach($error as $v){
					$xo .= $v;
				}
			}
			mysql_close();
		}
		$xo .= '
						<div class="jumbotron">
							<form method="post" action="" class="form-horizontal" role="form">
								<div class="form-group">
									<label for="materi" class="col-xs-12 col-sm-2 col-md-1 control-label">Materi</label>
									<div class="col-xs-12 col-sm-10 col-md-11">
										<input type="text" class="form-control" id="materi" name="materi" placeholder="Aljabar Linear dan Matriks" value="" autocomplete="off" required autofocus>
									</div>
								</div>
								<div class="form-group">
									<label for="tingkat" class="col-xs-12 col-sm-2 col-md-1 control-label">Tingkat</label>
									<div class="col-xs-12 col-sm-2 col-md-3">
										<input type="text" class="form-control" id="tingkat" name="tingkat" placeholder="SMA" value="" autocomplete="off" data-role="tagsinput">
									</div>
									<span> 
										<label for="jurusan" class="col-xs-12 col-sm-2 col-md-1 control-label">Jurusan</label>
										<div class="col-xs-12 col-sm-3 col-md-4">
											<input type="text" class="form-control" id="jurusan" name="jurusan" placeholder="IPA" value="" autocomplete="off" data-role="tagsinput">
										</div>
									</span>
									<span>
										<label for="kelas" class="col-xs-12 col-sm-1 control-label">Kelas</label>
										<div class="col-xs-12 col-sm-2 col-md-2">
											<input type="text" class="form-control" id="kelas" name="kelas" placeholder="10" value="" autocomplete="off" data-role="tagsinput">
										</div>
									</span>
								</div>
								<div class="form-group">
									<label for="tanggal" class="col-xs-12 col-sm-2 col-md-1 control-label">Tanggal</label>
									<div class="col-xs-12 col-sm-4 col-md-4">
										<input type="date" class="form-control datepicker" id="tanggal" name="tanggal" placeholder="'.date('Y-m-d').'" value="" autocomplete="off" required>
									</div>
									<span>
										<label for="jam" class="col-xs-12 col-sm-1 col-md-1 control-label">Jam</label>
										<div class="col-xs-12 col-sm-2 col-md-3 bootstrap-timepicker">
											<input type="time" class="form-control timepicker" id="jam" name="jam" placeholder="" value="" autocomplete="off" required>
										</div>
									</span>
									<span>
										<label for="kuota" class="col-xs-12 col-sm-1 control-label">Kuota</label>
										<div class="col-xs-12 col-sm-2">
											<input type="number" class="form-control" id="kuota" name="kuota" placeholder="45" value="" autocomplete="off" required>
										</div>
									</span>
								</div>
								<div class="col-xs-12 col-md-5 no-pad">
									<div class="form-group">
										<label for="status" class="col-xs-12 col-sm-2 col-md-2 pad-status control-label">Status</label>
										<div class="col-xs-12 col-sm-10 col-md-10 no-pad pad-div-status">
											<label class="checkbox-inline">
												<input type="radio" name="status" id="status" value="pengajar" checked>
												&nbsp;Pengajar
											</label>
											<label class="checkbox-inline">
												<input type="radio" name="status" id="status" value="pelajar">
												&nbsp;Pelajar
											</label>
										</div>
									</div>
									<div class="form-group">
										<label for="lokasi" class="col-xs-12 col-sm-2 col-md-2 control-label pad-lokasi">Lokasi</label>
										<div class="col-xs-12 col-sm-10 col-md-10 pad-div-lokasi">
											<input type="text" class="form-control" id="lokasi" name="lokasi" placeholder="Dago" value="" autocomplete="off">
										</div>
									</div>
								</div>
								<div class="col-xs-12 col-md-7 no-pad">
									<div class="form-group">
										<label for="alamat" class="col-xs-12 col-sm-2 col-md-2 pad-alamat control-label">Alamat</label>
										<div class="col-xs-12 col-sm-10 col-md-10 pad-div-alamat">
											<textarea class="form-control" rows="3" id="alamat" name="alamat" placeholder="Jalan Dipatiukur No. 123 Bandung" value="" autocomplete="off" onkeyup="codeAddress()" onchange="setAddress()" required></textarea>
										</div>
									</div>
								</div>
								<div class="form-group">
									<div class="col-sm-offset-2 col-sm-10 col-md-offset-1 col-md-offset-11">
										<button type="submit" class="btn btn-primary" name="submit">Buat Kelas</button>
									</div>
								</div>
							</form>
						</div>
						<!-- Google Map API	-->
						<script src="https://maps.googleapis.com/maps/api/js?v=3.exp"></script>
						<script src="./js/gmaps.js"></script>
						<script>init(\'x\');</script>
						<div id="map-canvas"></div>';
		echo $xo;
	}
	function kelas(){
		mysql_open();
		if(isset($_GET['mengajar']) OR isset($_GET['belajar']) OR isset($_GET['batal'])){
			$sql = "SELECT `ID` FROM `User` WHERE SHA1(`ID`)='".$_GET['user']."' LIMIT 1";
			$res = @mysql_query($sql) or die(mysql_error());
			$sql_ = "SELECT `ID` FROM `Kelas` WHERE SHA1(`ID`)='".$_GET['kelas']."' LIMIT 1";
			$res_ = @mysql_query($sql_) or die(mysql_error());
			if(mysql_num_rows($res) == 1 AND mysql_num_rows($res_) == 1){
				$row = mysql_fetch_assoc($res);
				$row_ = mysql_fetch_assoc($res_);
				if(isset($_GET['mengajar'])){
					$sql = "UPDATE `Belajar` SET `Status`='Pengajar' WHERE `ID Kelas`=".$row_['ID']." AND `ID User`=".$row['ID']." LIMIT 1";
					$res = @mysql_query($sql) or die(mysql_error());
					if(mysql_affected_rows() == 0){
						$sql = "INSERT INTO `Belajar` VALUES (".$row_['ID'].",".$row['ID'].",'Pengajar')";
						$res = mysql_query($sql) or die(mysql_error());
					}
				}elseif(isset($_GET['belajar'])){
					$sql = "UPDATE `Belajar` SET `Status`='Pelajar' WHERE `ID Kelas`=".$row_['ID']." AND `ID User`=".$row['ID']." LIMIT 1";
					$res = @mysql_query($sql) or die(mysql_error());
					if(mysql_affected_rows() == 0){
						$sql = "INSERT INTO `Belajar` VALUES (".$row_['ID'].",".$row['ID'].",'Pelajar')";
						$res = mysql_query($sql) or die(mysql_error());
					}
				}else{
					$sql = "DELETE FROM `Belajar` WHERE `ID Kelas`=".$row_['ID']." AND `ID User`=".$row['ID']." LIMIT 1";
					$res = @mysql_query($sql) or die(mysql_error());
				}
				$sql = "UPDATE `Kelas` SET `Terisi`=(SELECT COUNT(*) FROM `Belajar` WHERE `ID Kelas`=".$row_['ID']." AND `Status`='Pelajar') WHERE `ID`=".$row_['ID']." LIMIT 1";
				$res = @mysql_query($sql) or die(mysql_error());
			}
			header('Location: '.$_SERVER['HTTP_REFERER']);
			exit();
		}
		$sql = "SELECT * FROM `Kelas` WHERE SHA1(`ID`)='".$_GET['kelas']."' LIMIT 1";
		$res = @mysql_query($sql) or die(mysql_error());
		if(mysql_num_rows($res) == 1){
			$row = mysql_fetch_assoc($res);
			$sql_ = "SELECT * FROM `Materi` WHERE `ID`=".$row['ID Materi']." LIMIT 1";
			$res_ = @mysql_query($sql_) or die(mysql_error());
			if(mysql_num_rows($res_) == 1){
				$row_ = mysql_fetch_assoc($res_);
			}
			$sql__ = "SELECT * FROM `Belajar` WHERE `ID Kelas`=".$row['ID']." AND `Status`='Pengajar'";
			$res__ = @mysql_query($sql__) or die(mysql_error());
			if(mysql_num_rows($res__) > 0){
				while($row__ = mysql_fetch_assoc($res__)){
					$sql___ = "SELECT * FROM `User` WHERE `ID`=".$row__['ID User']." LIMIT 1";
					$res___ = @mysql_query($sql___) or die(mysql_error());
					if(mysql_num_rows($res___) == 1){
						$row___ = mysql_fetch_assoc($res___);
						if($row___['ID'] == $_SESSION['imancha']['id']){$bPengajar = true;}
						$pengajar .= '<tr>
														<td>'.++$i.'</td>
														<td>
															'.($row['Pembuat']==$row___['ID'] ? '<a href="./?profile='.$row___['Username'].'" style="color:#333;" data-toggle="tooltip" title="Pembuat Kelas">'.$row___['Nama'].'</a><span class="pull-right fa fa-thumb-tack"></span>' : '<a href="./?profile='.$row___['Username'].'" style="color:#333;">'.$row___['Nama'].'</a>').'
														</td>
													</tr>';
					}
				}
			}
			$sql__ = "SELECT * FROM `Belajar` WHERE `ID Kelas`=".$row['ID']." AND `Status`='Pelajar'";
			$res__ = @mysql_query($sql__) or die(mysql_error());
			if(mysql_num_rows($res__) > 0){
				while($row__ = mysql_fetch_assoc($res__)){
					$sql___ = "SELECT * FROM `User` WHERE `ID`=".$row__['ID User']." LIMIT 1";
					$res___ = @mysql_query($sql___) or die(mysql_error());
					if(mysql_num_rows($res___) == 1){
						$row___ = mysql_fetch_assoc($res___);
						if($row___['ID'] == $_SESSION['imancha']['id']){$bPelajar = true;}
						$pelajar .= '<tr>
														<td>'.++$j.'</td>
														<td>'.($row['Pembuat']==$row___['ID'] ? '<a href="./?profile='.$row___['Username'].'" style="color:#333;" data-toggle="tooltip" title="Pembuat Kelas">'.$row___['Nama'].'</a><span class="pull-right fa fa-thumb-tack"></span>' : '<a href="./?profile='.$row___['Username'].'" style="color:#333;">'.$row___['Nama'].'</a>').'</td>
													</tr>';
					}
				}
			}
			for($k=$j+1; $k<=$row['Kuota']; $k++){
				$pelajar .= '<tr>
												<td>'.$k.'</td>
												<td></td>
										 </tr>';
			}
			if($bPengajar){
				$bPengajar = '<div class="pull-right" style="margin-bottom:15px;">
												<button class="btn btn-warning btn-flat" onclick="window.location=\'./?kelas='.$_GET['kelas'].'&user='.SHA1($_SESSION['imancha']['id']).'&batal\'">Batal Mengajar</button>
											</div>';
			}else{
				$bPengajar = '<div class="pull-right" style="margin-bottom:15px;">
												<button class="btn btn-info btn-flat" '.(isset($_SESSION['imancha']['id']) ? 'onclick="window.location=\'./?kelas='.$_GET['kelas'].'&user='.SHA1($_SESSION['imancha']['id']).'&mengajar\'"' : 'data-toggle="modal" data-target="#myModal"').'>Ikut Mengajar</button>
											</div>';
			}
			if($bPelajar){
				$bPelajar = '<div class="pull-right" style="margin-bottom:15px;">
											<button class="btn btn-warning btn-flat" onclick="window.location=\'./?kelas='.$_GET['kelas'].'&user='.SHA1($_SESSION['imancha']['id']).'&batal\'">Batal Belajar</button>
										</div>';
			}else{
				$sql__ = "SELECT `Kuota`,`Terisi` FROM `Kelas` WHERE `ID`=".$row['ID']." LIMIT 1";
				$res__ = @mysql_query($sql__) or die(mysql_error());
				if(mysql_num_rows($res__) == 1){
					$row__ = mysql_fetch_assoc($res__);
					if($row__['Kuota'] != $row__['Terisi']){
						$bPelajar = '<div class="pull-right" style="margin-bottom:15px;">
													<button class="btn btn-success btn-flat" '.(isset($_SESSION['imancha']['id']) ? 'onclick="window.location=\'./?kelas='.$_GET['kelas'].'&user='.SHA1($_SESSION['imancha']['id']).'&belajar\'"' : 'data-toggle="modal" data-target="#myModal"').'>Ikut Belajar</button>
												</div>';
					}
				}
			}
			$xo .= '<div class="panel panel-primary">
								<div class="panel-heading">
									<div class="row">
										<div class="col-xs-12 col-sm-4 col-sm-push-8 text-right">
											<h3 class="panel-title">'.tanggal($row['Tanggal']).' '.substr($row['Jam'],0,5).'</h3>
										</div>
										<div class="col-xs-12 col-sm-8 col-sm-pull-4">
											<h3 class="panel-title">'.$row_['Nama'].'</h3>
										</div>
									</div>
								</div>
								<div class="panel-body">
									<div class="row">
										<div class="col-xs-12 col-md-7">
											<div class="row">
												<div class="col-xs-12 col-md-6">
													<table class="table table-striped table-condensed table-responsive">
														<thead>
															<tr>
																<th>#</th>
																<th>Pengajar</th>
															</tr>
														</thead>
														<tbody>
															'.$pengajar.'
														</tbody>
													</table>
													'.$bPengajar.'
												</div>
												<div class="col-xs-12 col-md-6">
													<table class="table table-striped table-condensed table-responsive table-hover">
														<thead>
															<tr>
																<th>#</th>
																<th>Pelajar</th>
															</tr>
														</thead>
														<tbody>
															'.$pelajar.'
														</tbody>
													</table>
													'.$bPelajar.'
												</div>
											</div>
										</div>
										<div class="col-xs-12 col-md-5">
											<table class="table table-condensed table-responsive">
												<thead>
													<tr>
														<th>Alamat : <span style="font-weight:normal">'.$row['Alamat'].'</span></th>
													</tr>
												</thead>
											</table>
											<div class="sr-only">
												<input type="text" id="alamat" value="'.$row['Alamat'].'">
											</div>
											<!-- Google Map API	-->
											<script src="https://maps.googleapis.com/maps/api/js?v=3.exp"></script>
											<script src="./js/gmaps.js"></script>
											<script>
												init(\'y\');
											</script>
											<div id="map-canvas"></div>
										</div>
									</div>
								</div>
							</div>
							<!-- Modal -->
							<div class="modal fade" id="myModal" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
								<div class="modal-dialog">
									<div class="modal-content">
										<div class="modal-header">
											<button type="button" class="close" data-dismiss="modal" aria-hidden="true">×</button>
											<h4 class="modal-title" id="myModalLabel">Oops!</h4>
										</div>
										<div class="modal-body">
											Silahkan <a href="./?masuk">masuk</a> atau <a href="./?daftar">daftar</a>.
										</div>
										<div class="modal-footer">
											<button type="button" class="btn btn-default" data-dismiss="modal">Close</button>
										</div>
									</div><!-- /.modal-content -->
								</div><!-- /.modal-dialog -->
							</div><!-- /.modal -->';
		}
		mysql_close();
		echo $xo;
	}
	function home(){
		mysql_open();
		$sql = "SELECT * FROM `Kelas` ORDER BY `ID` DESC";
		$res = @mysql_query($sql) or die(mysql_error());
		if(mysql_num_rows($res) > 0){
			$xo .= '<div class="row home">';
			while($row = mysql_fetch_assoc($res)){
				$sql_ = "SELECT * FROM `Materi` WHERE `ID`=".$row['ID Materi']." LIMIT 1";
				$res_ = @mysql_query($sql_) or die(mysql_error());
				if(mysql_num_rows($res_) == 1){
					$row_ = mysql_fetch_assoc($res_);
					$xo .= '<div class="col-xs-12 col-sm-6 col-md-4">
										<div class="panel panel-primary" onclick="window.location=\'./?kelas='.SHA1($row['ID']).'\'">
											<div class="panel-heading">
												<h3 class="panel-title">'.$row_['Nama'].'</h3>
											</div>
											<div class="panel-body">
												<div class="row">
													<div class="col-xs-12 col-sm-6 text-center b-xs" style="border-right:1px solid #eee;">
														<strong>'.tanggal($row['Tanggal']).'</strong>
														<h3>'.substr($row['Jam'],0,5).'</h3>
													</div>
													<div class="col-xs-12 col-sm-6 text-center">
														<h2>'.$row['Lokasi'].'</h2>
													</div>
												</div>
												<hr>
												<div class="row">
													<div class="col-xs-12">
														<p>Kapasitas : '.$row['Kuota'].' orang</p>
														<div class="progress">
															<div class="progress-bar progress-bar-success progress-bar-striped" style="width: '.(($row['Terisi']/$row['Kuota'])*100).'%" aria-valuemax="100" aria-valuemin="0" aria-valuenow="'.(($row['Terisi']/$row['Kuota'])*100).'" role="progressbar">
																<span class="sr-only">'.(($row['Terisi']/$row['Kuota'])*100).'% Complete (success)</span>
															</div>
														</div>
													</div>
												</div>
											</div>
										</div>
									</div>';
				}
			}
			$xo .= '</div>';
		}
		mysql_close();
		echo $xo;
	}
	function profile(){
		if(isset($_POST['submit'])){
			mysql_open();
			mysql_query("START TRANSACTION");
			if($_FILES['avatar']['size'] > 0){
				$target_dir = "./img/user/";
				$target_file = $target_dir . basename($_FILES["avatar"]["name"]);
				$uploadOk = 1;
				$imageFileType = pathinfo($target_file,PATHINFO_EXTENSION);
				$newfilename = mysql_real_escape_string(trim($_POST['id'])).'.'.$imageFileType;
				$target_file = $target_dir.$newfilename;
				$check = getimagesize($_FILES["avatar"]["tmp_name"]);
				if($check !== false){$uploadOk = 1;}
				else{$uploadOk = 0;
					echo '<div class="alert alert-warning">
									<button class="close" type="button" data-dismiss="alert">x</button>
									<strong>Oops! </strong>
									File bukan berupa gambar.
								</div>';
				}
				if($_FILES["avatar"]["size"] > 500000){$uploadOk = 0;
					echo '<div class="alert alert-warning">
									<button class="close" type="button" data-dismiss="alert">x</button>
									<strong>Oops! </strong>
									Ukuran file terlalu besar.
								</div>';
				}
				if($imageFileType != "png" AND $imageFileType != "jpg" AND $imageFileType != "gif"){$uploadOk = 0;
					echo '<div class="alert alert-warning">
									<button class="close" type="button" data-dismiss="alert">x</button>
									<strong>Oops! </strong>
									File harus berformat .png .jpg .gif
								</div>';
				}
				if($uploadOk == 0){$error = TRUE;}
				else{
					if(!move_uploaded_file($_FILES["avatar"]["tmp_name"], $target_file)){
						$error = TRUE;
						echo '<div class="alert alert-warning">
										<button class="close" type="button" data-dismiss="alert">x</button>
										<strong>Oops! </strong>
										Error upload gambar.
									</div>';
					}else{$avatar = ",`Avatar`='".$newfilename."'";}
				}
			}
			if(!empty($_POST['password'])){$password="`Password`=SHA1('".strip_tags(trim(mysql_real_escape_string($_POST['password'])))."'),";}
			$sql = "UPDATE `User` SET `Username`='".strip_tags(trim(mysql_real_escape_string($_POST['username'])))."',".$password."`Nama`='".strip_tags(trim(mysql_real_escape_string($_POST['nama'])))."',`Email`='".strip_tags(trim(mysql_real_escape_string($_POST['email'])))."',`Kota`='".strip_tags(trim(mysql_real_escape_string($_POST['kota'])))."',`Phone`='".strip_tags(trim(mysql_real_escape_string($_POST['phone'])))."'".$avatar." WHERE `ID`=".$_POST['id']." LIMIT 1";
			$res = @mysql_query($sql) or die(mysql_error());
			if(mysql_affected_rows() == 1){mysql_query("COMMIT");}else{mysql_query("ROLLBACK");}
			mysql_close();
		}
		mysql_open();
		$sql = "SELECT * FROM `User` WHERE `Username`='".$_GET['profile']."' LIMIT 1";
		$res = @mysql_query($sql) or die(mysql_error());
		if(mysql_num_rows($res) == 1){
			$row = mysql_fetch_assoc($res);
			if($_SESSION['imancha']['id'] == $row['ID']){
				$id = '<div class="form-group">
								<label class="col-sm-3 control-label">Username</label>
								<div class="col-sm-9">
									<input type="hidden" name="id" value="'.$row['ID'].'">
									<input class="form-control" type="text" id="username" name="username" placeholder="'.$row['Username'].'" value="'.$row['Username'].'" autocomplete="off" required>
								</div>
							</div>
							<div class="form-group">
								<label class="col-sm-3 control-label">Password</label>
								<div class="col-sm-9">
									<input class="form-control" type="password" id="password" name="password" placeholder="Password" value="" autocomplete="off">
								</div>
							</div>';
				$submit = '<div class="form-group">
										<div class="col-xs-12">
											<button type="submit" class="btn btn-warning pull-right" name="submit">Update Profile</button>
										</div>
									</div>';
				$upload = '<div class="input-group" style="padding-top:10px;">
											<div class="btn btn-file no-pad">
												<button class="btn btn-xs fileupload-new">Pilih Avatar</button>
												<button class="btn btn-xs fileupload-exists">Ganti</button>
												<input type="file" id="avatar" name="avatar">
											</div>
											<button class="btn btn-xs fileupload-exists" data-dismiss="fileupload">Hapus</button>
											<br>
											<div class="uneditable-input">
												<i class="fileupload-exists"></i>
												<div class="fileupload-preview"></div>
											</div>
										</div>';
			}else{
				$disabled = 'disabled';
			}
			$xo .= '<div class="jumbotron">
								<div class="row profile">
									<form class="form-horizontal" method="post" action="" enctype="multipart/form-data" role="form">
										<div class="col-xs-12 col-sm-3 col-md-2 col-md-offset-1 text-center">
											<div class="fileupload fileupload-new" data-provides="fileupload">
												<div data-toggle="tooltip" data-placement="bottom" title="imancha">
													<img id="img" class="img-thumbnail" src="./img/user/'.($row['Avatar']=="" ? "0.png" : $row['Avatar']).'"></img>
												</div>
												'.$upload.'
											</div>
										</div>
										<div class="col-xs-12 col-sm-9 col-md-8">
												'.$id.'
												<div class="form-group">
													<label class="col-sm-3 control-label">Nama</label>
													<div class="col-sm-9">
														<input class="form-control" type="text" id="nama" name="nama" placeholder="'.$row['Nama'].'" value="'.$row['Nama'].'" autocomplete="off" required '.$disabled.'>
													</div>
												</div>
												<div class="form-group">
													<label class="col-sm-3 control-label">Email</label>
													<div class="col-sm-9">
														<input class="form-control" type="email" id="email" name="email" placeholder="'.$row['Email'].'" value="'.$row['Email'].'" autocomplete="off" required '.$disabled.'>
													</div>
												</div>
												<div class="form-group">
													<label class="col-sm-3 control-label">Kota</label>
													<div class="col-sm-9">
														<input class="form-control" type="text" id="kota" name="kota" placeholder="'.$row['Kota'].'" value="'.$row['Kota'].'" autocomplete="off" required '.$disabled.'>
													</div>
												</div>
												<div class="form-group">
													<label class="col-sm-3 control-label">Phone</label>
													<div class="col-sm-9">
														<input class="form-control" type="text" id="phone" name="phone" placeholder="'.$row['Phone'].'" value="'.$row['Phone'].'" autocomplete="off" required '.$disabled.'>
													</div>
												</div>
												'.$submit.'
										</div>
									</form>
								</div>
							</div>';
		}
		mysql_close();
		echo $xo;
	}
	function timeline(){
		if(!isset($_SESSION['imancha']['id'])){header('Location: ./?masuk'); exit();}
		mysql_open();
		if(isset($_POST['submit'])){
			$sql = "INSERT INTO `Review` VALUES (".$_POST['kelas'].",".$_SESSION['imancha']['id'].",'".strip_tags(trim(mysql_real_escape_string($_POST['review'])))."',NOW())";
			$res = @mysql_query($sql) or die(mysql_error());
			if(mysql_affected_rows() == 1){
				header('Location: ./?timeline');
				exit();
			}else{
				$xo .= '<div class="alert alert-danger">
									<button class="close" type="button" data-dismiss="alert">x</button>
									<strong>Oops! </strong>
									Terjadi kesalahan. Silahkan coba lagi.
								</div>';
			}
		}
		$sql = "SELECT a.`ID`,a.`Tanggal`,b.`Nama` FROM `Kelas` a JOIN `Materi` b ON a.`ID Materi`=b.`ID` JOIN `Belajar` c ON a.`ID`=c.`ID Kelas` JOIN `User` d ON c.`ID User`=d.`ID` WHERE d.`ID`=".$_SESSION['imancha']['id']." ORDER BY a.`Tanggal` DESC";
		$res = @mysql_query($sql) or die(mysql_error());
		if(mysql_num_rows($res) > 0){
			$xo .= '<div class="row">
								<div class="col-md-12">
									<ul class="timeline">';
			while($row = mysql_fetch_assoc($res)){
				$sql_ = "SELECT * FROM `Review` WHERE `ID Kelas`=".$row['ID']." AND `ID User`=".$_SESSION['imancha']['id']." LIMIT 1";
				$res_ = @mysql_query($sql_) or die(mysql_error());
				if(mysql_num_rows($res_) == 1){
					$row_ = mysql_fetch_assoc($res_);
					$review = '<p>'.$row_['Review'].'</p>';
				}else{
					$review = '<form method="post" action="" role="form">
											<div class="form-group">
												<label for="name" class="sr-only">Text Area</label>
												<input type="hidden" name="kelas" value="'.$row['ID'].'">
												<textarea class="form-control btn-flat" rows="3" placeholder="Tulis Review . . ." name="review"></textarea>
											</div>
											<div class="form-group">
												<div class="pull-right" style="margin-bottom:25px;">
													<button class="btn btn-default btn-flat" type="submit" name="submit">Simpan</button>
												</div>
											</div>
										</form>';
				}
				$sql_ = "SELECT * FROM `Jurusan Kelas` WHERE `ID Kelas`=".$row['ID']."";
				$res_ = @mysql_query($sql_) or die(mysql_error());
				if(mysql_num_rows($res_) > 0){
					while($row_ = mysql_fetch_assoc($res_)){
						$sql__ = "SELECT * FROM `Jurusan` WHERE `ID`=".$row_['ID Jurusan']." LIMIT 1";
						$res__ = @mysql_query($sql__) or die(mysql_error());
						if(mysql_num_rows($res__) == 1){
							$row__ = mysql_fetch_assoc($res__);
							$inf[] = $row__['Nama'];
						}
					}
					$info = implode(', ',$inf);
					$inf = '';
				}else{
					$info = 'Umum';
				}
				$i++;
				$xo .= '<li'.($i%2==0 ? ' class="timeline-inverted"' : '').'>
									<div class="timeline-badge'.($i%2==0 ? ' invert' : '').' bg-gray"></div>
									<div class="timeline-info'.($i%2==0 ? '' : ' inverted').'">
										<div class="float-right">
											<div class="timeline-info-time">
												<div><i class="fa fa-tags'.($i%2==0 ? ' fa-flip-horizontal fa-md' : '').' c-gray"></i></div>
												<div class="c-gray timeline-info-hour">&nbsp;</div>
											</div>
											<div class="timeline-info-type c-gray'.($i%2==0 ? ' m-md' : '').'">'.$info.'</div>
										</div>
									</div>
									<div class="timeline-panel">
										<div class="timeline-heading">
											<div class="pull-left">
												<div class="timeline-day-number">'.substr($row['Tanggal'],8,2).'</div>
												<div>
													<div class="timeline-day">'.substr(tanggal($row['Tanggal']),3,strlen(tanggal($row['Tanggal']))-8).'</div>
													<div class="timeline-month c-gray">'.substr(tanggal($row['Tanggal']),strlen(tanggal($row['Tanggal']))-4,strlen(tanggal($row['Tanggal']))-3).'</div>
												</div>
											</div>
											<div class="pull-right">
												<div class="sr-only">
													<a href="#" rel="tooltip" data-toggle="tooltip" data-placement="top" title="Edit"><i class="fa fa-pencil-square-o"></i></a>
													<a href="#" rel="tooltip" data-toggle="tooltip" data-placement="top" title="Share"><i class="fa fa-share"></i></a>
													<a href="#" rel="tooltip" data-toggle="tooltip" data-placement="top" title="Remove"><i class="fa fa-times"></i></a>
												</div>
											</div>
										</div>
										<div class="timeline-body">
											<h4><strong>'.$row['Nama'].'</strong></h4>
											'.$review.'
										</div>
									</div>
								</li>';
			}
		}
		mysql_close();
		echo $xo;
	}
	function main(){
		if(isset($_GET['masuk'])){masuk();}
		elseif(isset($_GET['daftar'])){daftar();}
		elseif(isset($_GET['keluar'])){keluar();}
		elseif(isset($_GET['buatkelas'])){buatkelas();}
		elseif(isset($_GET['kelas'])){kelas();}
		elseif(isset($_GET['profile'])){profile();}
		elseif(isset($_GET['timeline'])){timeline();}
		else{home();}
	}
?>
