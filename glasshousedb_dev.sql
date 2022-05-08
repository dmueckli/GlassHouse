-- phpMyAdmin SQL Dump
-- version 5.1.3
-- https://www.phpmyadmin.net/
--
-- Host: localhost
-- Erstellungszeit: 08. Mai 2022 um 08:27
-- Server-Version: 10.5.15-MariaDB-0+deb11u1
-- PHP-Version: 7.4.28

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Datenbank: `glasshousedb_dev`
--
CREATE DATABASE IF NOT EXISTS `glasshousedb_dev` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;
USE `glasshousedb_dev`;

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `tbl_hosts`
--
-- Erstellt am: 01. Mai 2022 um 22:31
-- Zuletzt aktualisiert: 06. Mai 2022 um 21:07
--

CREATE TABLE `tbl_hosts` (
  `id` bigint(20) NOT NULL COMMENT 'Host ID - Primary Key',
  `name` varchar(255) NOT NULL COMMENT 'Host Name',
  `versionid` bigint(20) NOT NULL COMMENT 'Glasshouse version id the host is currently running.',
  `mac` varchar(17) NOT NULL COMMENT 'Host Mac Address',
  `local_ip` int(10) UNSIGNED NOT NULL COMMENT 'Hosts Local IP Address',
  `gateway_ip` int(10) UNSIGNED NOT NULL COMMENT 'Hosts Gateway IP Address'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- RELATIONEN DER TABELLE `tbl_hosts`:
--   `versionid`
--       `tbl_versions` -> `id`
--

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `tbl_sensordata`
--
-- Erstellt am: 01. Mai 2022 um 22:19
-- Zuletzt aktualisiert: 08. Mai 2022 um 06:27
--

CREATE TABLE `tbl_sensordata` (
  `id` bigint(20) NOT NULL COMMENT 'Internal entry id - primary key',
  `host_id` bigint(20) NOT NULL COMMENT 'Host id from which the data came',
  `user_id` bigint(20) NOT NULL COMMENT 'Users id.',
  `humidity` float(10,8) NOT NULL COMMENT 'Humidity inside the glasshouse.',
  `soil_moisture` int(3) NOT NULL COMMENT 'Soil moisture from the soil moisture sensor.',
  `temperature` float(10,8) NOT NULL COMMENT 'Temperature inside the glasshouse.',
  `heat_index` float(10,8) NOT NULL COMMENT 'Heat index inside the glasshouse.',
  `time` datetime DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- RELATIONEN DER TABELLE `tbl_sensordata`:
--   `host_id`
--       `tbl_hosts` -> `id`
--   `user_id`
--       `tbl_users` -> `id`
--

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `tbl_sessions`
--
-- Erstellt am: 01. Mai 2022 um 22:17
-- Zuletzt aktualisiert: 08. Mai 2022 um 06:20
--

CREATE TABLE `tbl_sessions` (
  `id` bigint(20) NOT NULL COMMENT 'Session unique id.',
  `userid` bigint(20) NOT NULL COMMENT 'User id.',
  `accesstoken` varchar(100) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL COMMENT 'Access token.',
  `accesstokenexpiry` datetime NOT NULL COMMENT 'Access token expiry date/time.',
  `refreshtoken` varchar(100) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL COMMENT 'Refresh token.',
  `refreshtokenexpiry` datetime NOT NULL COMMENT 'Refresh token expiry date/time.'
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Sessions table.';

--
-- RELATIONEN DER TABELLE `tbl_sessions`:
--   `userid`
--       `tbl_users` -> `id`
--

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `tbl_users`
--
-- Erstellt am: 01. Mai 2022 um 22:17
-- Zuletzt aktualisiert: 05. Mai 2022 um 13:05
--

CREATE TABLE `tbl_users` (
  `id` bigint(20) NOT NULL COMMENT 'Internal user ID.',
  `firstname` varchar(255) NOT NULL COMMENT 'Users first name.',
  `surname` varchar(255) NOT NULL COMMENT 'Users surname.',
  `username` varchar(255) NOT NULL COMMENT 'Users unique username.',
  `password` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL COMMENT 'Users password.',
  `active` enum('N','Y') NOT NULL DEFAULT 'Y' COMMENT 'Is the user active.',
  `login_attempts` int(1) NOT NULL DEFAULT 0 COMMENT 'Failed login attempts.',
  `type` enum('USER','ESP32','MACHINE','') NOT NULL DEFAULT 'USER',
  `hostid` bigint(20) DEFAULT NULL COMMENT 'Host id.'
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Users table.';

--
-- RELATIONEN DER TABELLE `tbl_users`:
--   `hostid`
--       `tbl_hosts` -> `id`
--

-- --------------------------------------------------------

--
-- Tabellenstruktur für Tabelle `tbl_versions`
--
-- Erstellt am: 05. Mai 2022 um 16:55
-- Zuletzt aktualisiert: 05. Mai 2022 um 18:11
--

CREATE TABLE `tbl_versions` (
  `id` bigint(20) NOT NULL COMMENT 'Version ID.',
  `name` varchar(255) NOT NULL COMMENT 'Version name.',
  `description` varchar(255) DEFAULT NULL COMMENT 'Version short description.',
  `active` enum('Y','N') NOT NULL DEFAULT 'Y' COMMENT 'Version is active'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- RELATIONEN DER TABELLE `tbl_versions`:
--

-- --------------------------------------------------------

--
-- Stellvertreter-Struktur des Views `WeatherData`
-- (Siehe unten für die tatsächliche Ansicht)
--
CREATE TABLE `WeatherData` (
`id` bigint(20)
,`hostid` bigint(20)
,`hostname` varchar(255)
,`version` varchar(255)
,`mac` varchar(17)
,`local_ip` varchar(31)
,`gateway_ip` varchar(31)
,`humidity` float(10,8)
,`soil_moisture` int(3)
,`temperature` float(10,8)
,`heat_index` float(10,8)
,`time` datetime
);

-- --------------------------------------------------------

--
-- Struktur des Views `WeatherData`
--
DROP TABLE IF EXISTS `WeatherData`;

CREATE ALGORITHM=UNDEFINED DEFINER=`muecklichd`@`%` SQL SECURITY DEFINER VIEW `WeatherData`  AS SELECT `tbl_sensordata`.`id` AS `id`, `tbl_hosts`.`id` AS `hostid`, `tbl_hosts`.`name` AS `hostname`, `tbl_versions`.`name` AS `version`, `tbl_hosts`.`mac` AS `mac`, inet_ntoa(`tbl_hosts`.`local_ip`) AS `local_ip`, inet_ntoa(`tbl_hosts`.`gateway_ip`) AS `gateway_ip`, `tbl_sensordata`.`humidity` AS `humidity`, `tbl_sensordata`.`soil_moisture` AS `soil_moisture`, `tbl_sensordata`.`temperature` AS `temperature`, `tbl_sensordata`.`heat_index` AS `heat_index`, `tbl_sensordata`.`time` AS `time` FROM ((`tbl_sensordata` join `tbl_hosts` on(`tbl_sensordata`.`host_id` = `tbl_hosts`.`id`)) join `tbl_versions` on(`tbl_hosts`.`versionid` = `tbl_versions`.`id`)) ORDER BY `tbl_sensordata`.`id` AS `DESCdesc` ASC  ;

--
-- Indizes der exportierten Tabellen
--

--
-- Indizes für die Tabelle `tbl_hosts`
--
ALTER TABLE `tbl_hosts`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `mac` (`mac`),
  ADD UNIQUE KEY `name` (`name`),
  ADD KEY `hostsversions_fk` (`versionid`);

--
-- Indizes für die Tabelle `tbl_sensordata`
--
ALTER TABLE `tbl_sensordata`
  ADD PRIMARY KEY (`id`),
  ADD KEY `cst_host_id` (`host_id`),
  ADD KEY `weatherdatauserid_fk` (`user_id`);

--
-- Indizes für die Tabelle `tbl_sessions`
--
ALTER TABLE `tbl_sessions`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `accesstoken` (`accesstoken`),
  ADD UNIQUE KEY `refreshtoken` (`refreshtoken`),
  ADD KEY `sessionuserid_fk` (`userid`);

--
-- Indizes für die Tabelle `tbl_users`
--
ALTER TABLE `tbl_users`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `username` (`username`),
  ADD KEY `usershostid_fk` (`hostid`);

--
-- Indizes für die Tabelle `tbl_versions`
--
ALTER TABLE `tbl_versions`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT für exportierte Tabellen
--

--
-- AUTO_INCREMENT für Tabelle `tbl_hosts`
--
ALTER TABLE `tbl_hosts`
  MODIFY `id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT 'Host ID - Primary Key';

--
-- AUTO_INCREMENT für Tabelle `tbl_sensordata`
--
ALTER TABLE `tbl_sensordata`
  MODIFY `id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT 'Internal entry id - primary key';

--
-- AUTO_INCREMENT für Tabelle `tbl_sessions`
--
ALTER TABLE `tbl_sessions`
  MODIFY `id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT 'Session unique id.';

--
-- AUTO_INCREMENT für Tabelle `tbl_users`
--
ALTER TABLE `tbl_users`
  MODIFY `id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT 'Internal user ID.';

--
-- AUTO_INCREMENT für Tabelle `tbl_versions`
--
ALTER TABLE `tbl_versions`
  MODIFY `id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT 'Version ID.';

--
-- Constraints der exportierten Tabellen
--

--
-- Constraints der Tabelle `tbl_hosts`
--
ALTER TABLE `tbl_hosts`
  ADD CONSTRAINT `hostsversions_fk` FOREIGN KEY (`versionid`) REFERENCES `tbl_versions` (`id`);

--
-- Constraints der Tabelle `tbl_sensordata`
--
ALTER TABLE `tbl_sensordata`
  ADD CONSTRAINT `weatherdatahostid_fk` FOREIGN KEY (`host_id`) REFERENCES `tbl_hosts` (`id`),
  ADD CONSTRAINT `weatherdatauserid_fk` FOREIGN KEY (`user_id`) REFERENCES `tbl_users` (`id`);

--
-- Constraints der Tabelle `tbl_sessions`
--
ALTER TABLE `tbl_sessions`
  ADD CONSTRAINT `sessionuserid_fk` FOREIGN KEY (`userid`) REFERENCES `tbl_users` (`id`);

--
-- Constraints der Tabelle `tbl_users`
--
ALTER TABLE `tbl_users`
  ADD CONSTRAINT `usershostid_fk` FOREIGN KEY (`hostid`) REFERENCES `tbl_hosts` (`id`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
