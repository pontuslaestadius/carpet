 // KeyCode or 'c'haracter, field, messageid, [down value, up value], label.
registerkey(); // This is not displayed
//registerkey(38, "forward", 1041, [0.25, 0.00]);
registerkey(38, "percent", 1041, ["range", 0.00]);
registerkey(); // This is not displayed
registerkey(37, "steeringAngle", 1045, [-15, 0], "left");
registerkey(40, "reverse", 1041, [-0.25, 0.00]);
registerkey(39, "steeringAngle", 1045, [15, 0], "right");

registerkey();registerkey();registerkey();

registerkey('a', "vehicleIp", 1001, ["\"192.168.43.135\", \"groupId\":\"6\"", null], "[a]p");
registerkey('f', "", 1002, [null, null], "[f]req");
registerkey('r', "", 1003, [null, null], "f[r]es");
registerkey('s', "", 1004, [null, null], "[s]tpf");
registerkey('o', "", 3001, [null, null], "f[o]ls");

//registerkey('e', "timestamp", 1001, ["\"192.168.43.135\", \"groupId\":\"6\"", null], "l[e]ds");

/*
message LeaderStatus [id = 2001] {
  uint32 timestamp [id = 1];
  float speed [id = 2];
  float steeringAngle [id = 3];
  uint8 distanceTraveled [id = 4];
}
*/