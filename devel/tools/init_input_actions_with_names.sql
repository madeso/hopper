

insert into input values ( 0, 0, "Escape", 'false', 1, 0, 0, 1, 0, 'false', 0 ); -- Esc : released key 1
insert into input values ( 0, 1, "Pause" , 1, 0, 0, 197, 0, 'false', 0 ); -- pause : released key pause
insert into input values ( 0, 2, "Mouse click 0", 'false', 1, 1, 0, 0, 0, 'false', 0 ); -- MB1 released
insert into input values ( 0, 3, "Mouse click 1", 'false', 1, 1, 0, 1, 0, 'false', 0 ); -- MB2 released
insert into input values ( 0, 4, "Space", 'false', 1, 0, 0, 57, 0, 'false', 0 ); -- space released
insert into input values ( 0, 5, "Space", 'false', 1, 0, 0, 57, 0, 'false', 0 ); -- toogle = space
insert into input values ( 0, 6, "Screenshot", 'false', 1, 0, 0, 88, 0, 'false', 0 ); -- F12 released
insert into input values ( 0, 7, "New Game : Landing", 'false', 1, 0, 0, 59, 0, 'false', 0 ); -- landing = F1 released
insert into input values ( 0, 8, "New Game : Racing", 'false', 1, 0, 0, 60, 0, 'false', 0 ); -- racing = F2 released
insert into input values ( 0, 9, "New Game : Tutorial", 'false', 1, 0, 0, 61, 0, 'false', 0 ); -- tutorial = F3 released
insert into input values ( 0, 10, "Debug ODE meshes", 'false', 1, 0, 0, 64, 0, 'false', 0 ); -- debugODE = F6 released
insert into input values ( 0, 11, "Debug Bounding Boxes", 'false', 1, 0, 0, 63, 0, 'false', 0 ); -- debugBB = F5 released
insert into input values ( 0, 12, "Editor : Save", 'false', 1, 0, 0, 68, 0, 'false', 0 ); -- save(editor)=F10 released
--yaw : axis keys Q D (azerty) reversed configurable
insert into input values ( 0, 13, "Yaw", 'true', 2, 0, 0, 30, 32, 'true', 0 );
-- pitch : axis mouse Yaxis configurable
insert into input values ( 0, 14, "Pitch", 'true', 2, 3, 0, 1, 0, 'false', 0 );
-- roll : axis mouse Xaxis configurable
insert into input values ( 0, 15, "Roll", 'true', 2, 3, 0, 0, 0, 'false', 0 );
-- campitch : axis mouse Zaxis configurable
insert into input values ( 0, 16, "Camera Pitch", 'true', 2, 3, 0, 2, 0, 'false', 0 );
-- thrust : pressed mousebutton 0 configurable
insert into input values ( 0, 17, "Thrust", 'true', 0, 1, 0, 0, 0, 'false', 0 );
-- recover : pressed mousebutton 1 configurable
insert into input values ( 0, 18, "Recover", 'true', 0, 1, 0, 1, 0, 'false', 0 );
-- pressedMB1 : pressed mousebutton 0
insert into input values ( 0, 19, "Mouse Button 0 pressed", 'false', 0, 1, 0, 0, 0, 'false', 0 );
-- forward : pressed key Z (azerty)
insert into input values ( 0, 20, "Editor : Forward", 'false', 0, 0, 0, 17, 0, 'false', 0 );
-- backward : pressed key S (azerty)
insert into input values ( 0, 21, "Editor : Backward", 'false', 0, 0, 0, 31, 0, 'false', 0 );
-- left : pressed key Q (azerty)
insert into input values ( 0, 22, "Editor : Left", 'false', 0, 0, 0, 30, 0, 'false', 0 );
-- right : pressed key D ( azerty )
insert into input values ( 0, 23, "Editor : Right", 'false', 0, 0, 0, 32, 0, 'false', 0 );
-- editor accel : pressed key LeftShift
insert into input values ( 0, 24, "Editor : Accel", 'false', 0, 0, 0, 42, 0, 'false', 0 );
