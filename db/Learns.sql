-- phpMyAdmin SQL Dump
-- version 4.0.10deb1
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Feb 04, 2015 at 09:39 PM
-- Server version: 5.5.41-0ubuntu0.14.04.1
-- PHP Version: 5.5.9-1ubuntu4.5

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Database: `Learns`
--

-- --------------------------------------------------------

--
-- Table structure for table `Belajar`
--

CREATE TABLE IF NOT EXISTS `Belajar` (
  `ID Kelas` int(11) NOT NULL,
  `ID User` int(11) NOT NULL,
  `Status` enum('Pengajar','Pelajar') NOT NULL,
  KEY `ID Kelas` (`ID Kelas`,`ID User`),
  KEY `ID User` (`ID User`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `Belajar`
--

INSERT INTO `Belajar` (`ID Kelas`, `ID User`, `Status`) VALUES
(4, 1, 'Pelajar'),
(3, 1, 'Pengajar'),
(2, 1, 'Pelajar'),
(1, 1, 'Pengajar');

-- --------------------------------------------------------

--
-- Table structure for table `Jurusan`
--

CREATE TABLE IF NOT EXISTS `Jurusan` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `Nama` varchar(45) NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=4 ;

--
-- Dumping data for table `Jurusan`
--

INSERT INTO `Jurusan` (`ID`, `Nama`) VALUES
(1, 'IPA'),
(2, 'IPS'),
(3, 'Bahasa');

-- --------------------------------------------------------

--
-- Table structure for table `Jurusan Kelas`
--

CREATE TABLE IF NOT EXISTS `Jurusan Kelas` (
  `ID Kelas` int(11) NOT NULL,
  `ID Jurusan` int(11) NOT NULL,
  KEY `ID Kelas` (`ID Kelas`),
  KEY `ID Jurusan` (`ID Jurusan`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `Jurusan Kelas`
--

INSERT INTO `Jurusan Kelas` (`ID Kelas`, `ID Jurusan`) VALUES
(1, 1),
(1, 2),
(1, 3),
(2, 1),
(2, 2),
(2, 3);

-- --------------------------------------------------------

--
-- Table structure for table `Kelas`
--

CREATE TABLE IF NOT EXISTS `Kelas` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `Tanggal` date NOT NULL,
  `Jam` time NOT NULL,
  `Lokasi` varchar(45) NOT NULL,
  `Alamat` text NOT NULL,
  `Kuota` int(11) NOT NULL,
  `Terisi` int(11) NOT NULL,
  `Pembuat` int(11) NOT NULL,
  `ID Materi` int(11) NOT NULL,
  PRIMARY KEY (`ID`),
  KEY `ID Materi` (`ID Materi`),
  KEY `Pembuat` (`Pembuat`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=5 ;

--
-- Dumping data for table `Kelas`
--

INSERT INTO `Kelas` (`ID`, `Tanggal`, `Jam`, `Lokasi`, `Alamat`, `Kuota`, `Terisi`, `Pembuat`, `ID Materi`) VALUES
(1, '2015-01-31', '20:45:00', 'Dago', 'Jalan Dipatiukur No.123 Bandung', 11, 0, 1, 1),
(2, '2015-02-01', '22:00:00', 'Dago', 'Jalan Dipatiukur No. 123 Bandung', 20, 1, 1, 2),
(3, '2015-02-02', '22:15:00', 'Pusdai', 'Jalan Suci No. 234 Bandung', 12, 0, 1, 3),
(4, '2015-02-03', '22:15:00', 'Gasibu', 'Jalan Japati No. 345 Bandung', 11, 1, 1, 4);

-- --------------------------------------------------------

--
-- Table structure for table `Materi`
--

CREATE TABLE IF NOT EXISTS `Materi` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `Nama` varchar(45) NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=5 ;

--
-- Dumping data for table `Materi`
--

INSERT INTO `Materi` (`ID`, `Nama`) VALUES
(1, 'Pemrograman Dasar'),
(2, 'Pemrograman Lanjut'),
(3, 'Matematika'),
(4, 'Bahasa Inggris');

-- --------------------------------------------------------

--
-- Table structure for table `Review`
--

CREATE TABLE IF NOT EXISTS `Review` (
  `ID Kelas` int(11) NOT NULL,
  `ID User` int(11) NOT NULL,
  `Review` text NOT NULL,
  `Waktu` datetime NOT NULL,
  KEY `ID Kelas` (`ID Kelas`),
  KEY `ID User` (`ID User`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `Review`
--

INSERT INTO `Review` (`ID Kelas`, `ID User`, `Review`, `Waktu`) VALUES
(4, 1, 'Ini adalah sebuah review untuk kelas `Bahasa Inggris`', '2015-02-04 13:01:20');

-- --------------------------------------------------------

--
-- Table structure for table `Semester`
--

CREATE TABLE IF NOT EXISTS `Semester` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `Nama` int(11) NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=4 ;

--
-- Dumping data for table `Semester`
--

INSERT INTO `Semester` (`ID`, `Nama`) VALUES
(1, 10),
(2, 11),
(3, 12);

-- --------------------------------------------------------

--
-- Table structure for table `Semester Kelas`
--

CREATE TABLE IF NOT EXISTS `Semester Kelas` (
  `ID Kelas` int(11) NOT NULL,
  `ID Semester` int(11) NOT NULL,
  KEY `ID Kelas` (`ID Kelas`,`ID Semester`),
  KEY `ID Semester` (`ID Semester`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `Semester Kelas`
--

INSERT INTO `Semester Kelas` (`ID Kelas`, `ID Semester`) VALUES
(1, 1),
(1, 2),
(1, 3),
(2, 1),
(2, 2),
(2, 3);

-- --------------------------------------------------------

--
-- Table structure for table `Tingkat`
--

CREATE TABLE IF NOT EXISTS `Tingkat` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `Nama` varchar(45) NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=6 ;

--
-- Dumping data for table `Tingkat`
--

INSERT INTO `Tingkat` (`ID`, `Nama`) VALUES
(1, 'SMA'),
(2, 'MA'),
(3, 'SMK'),
(4, 'SMP'),
(5, 'MTS');

-- --------------------------------------------------------

--
-- Table structure for table `Tingkat Kelas`
--

CREATE TABLE IF NOT EXISTS `Tingkat Kelas` (
  `ID Kelas` int(11) NOT NULL,
  `ID Tingkat` int(11) NOT NULL,
  KEY `ID Kelas` (`ID Kelas`,`ID Tingkat`),
  KEY `ID Tingkat` (`ID Tingkat`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `Tingkat Kelas`
--

INSERT INTO `Tingkat Kelas` (`ID Kelas`, `ID Tingkat`) VALUES
(1, 1),
(1, 2),
(1, 3),
(2, 1),
(2, 2),
(2, 3),
(3, 4),
(3, 5);

-- --------------------------------------------------------

--
-- Table structure for table `User`
--

CREATE TABLE IF NOT EXISTS `User` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `Username` varchar(45) NOT NULL,
  `Password` varchar(45) NOT NULL,
  `Nama` varchar(45) NOT NULL,
  `Email` varchar(45) NOT NULL,
  `Kota` varchar(45) NOT NULL,
  `Phone` varchar(15) NOT NULL,
  `Waktu` datetime NOT NULL,
  `Avatar` varchar(10) NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=2 ;

--
-- Dumping data for table `User`
--

INSERT INTO `User` (`ID`, `Username`, `Password`, `Nama`, `Email`, `Kota`, `Phone`, `Waktu`, `Avatar`) VALUES
(1, 'imancha', 'eb2f3b2278c1eb606a02a227fce6b51a1d1fec4e', 'Mohammad Abdul Iman Syah', 'imancha_266@ymail.com', 'Cirebon', '085224057100', '2015-01-30 15:11:20', '1.png');

--
-- Constraints for dumped tables
--

--
-- Constraints for table `Belajar`
--
ALTER TABLE `Belajar`
  ADD CONSTRAINT `Belajar_ibfk_2` FOREIGN KEY (`ID User`) REFERENCES `User` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `Belajar_ibfk_1` FOREIGN KEY (`ID Kelas`) REFERENCES `Kelas` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `Jurusan Kelas`
--
ALTER TABLE `Jurusan Kelas`
  ADD CONSTRAINT `Jurusan Kelas_ibfk_1` FOREIGN KEY (`ID Kelas`) REFERENCES `Kelas` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `Jurusan Kelas_ibfk_2` FOREIGN KEY (`ID Jurusan`) REFERENCES `Jurusan` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `Kelas`
--
ALTER TABLE `Kelas`
  ADD CONSTRAINT `Kelas_ibfk_1` FOREIGN KEY (`Pembuat`) REFERENCES `User` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `Kelas_ibfk_2` FOREIGN KEY (`ID Materi`) REFERENCES `Materi` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `Review`
--
ALTER TABLE `Review`
  ADD CONSTRAINT `Review_ibfk_2` FOREIGN KEY (`ID User`) REFERENCES `User` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `Review_ibfk_1` FOREIGN KEY (`ID Kelas`) REFERENCES `Kelas` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `Semester Kelas`
--
ALTER TABLE `Semester Kelas`
  ADD CONSTRAINT `Semester Kelas_ibfk_1` FOREIGN KEY (`ID Kelas`) REFERENCES `Kelas` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `Semester Kelas_ibfk_2` FOREIGN KEY (`ID Semester`) REFERENCES `Semester` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `Tingkat Kelas`
--
ALTER TABLE `Tingkat Kelas`
  ADD CONSTRAINT `Tingkat Kelas_ibfk_1` FOREIGN KEY (`ID Kelas`) REFERENCES `Kelas` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `Tingkat Kelas_ibfk_2` FOREIGN KEY (`ID Tingkat`) REFERENCES `Tingkat` (`ID`) ON DELETE CASCADE ON UPDATE CASCADE;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
