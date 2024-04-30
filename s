[33mcommit 3539819e15f109fcf8970519903bf24823f9a71f[m
Author: Nathan Song <szj952261968@outlook.com>
Date:   Tue Apr 30 18:26:30 2024 +0800

    Delete models

[1mdiff --git a/Rescources/models/CornellBox/CornellBox-Empty-CO.mtl b/Rescources/models/CornellBox/CornellBox-Empty-CO.mtl[m
[1mdeleted file mode 100644[m
[1mindex a0c523a..0000000[m
[1m--- a/Rescources/models/CornellBox/CornellBox-Empty-CO.mtl[m
[1m+++ /dev/null[m
[36m@@ -1,71 +0,0 @@[m
[31m-# An empty Cornell Box with cyan and orange walls in OBJ format.[m
[31m-# Note that the real box is not a perfect cube, so[m
[31m-# the faces are imperfect in this data set.[m
[31m-#[m
[31m-# Created by Guedis Cardenas and Morgan McGuire at Williams College, 2011[m
[31m-# Released into the Public Domain.[m
[31m-#[m
[31m-# http://graphics.cs.williams.edu/data[m
[31m-# http://www.graphics.cornell.edu/online/box/data.html[m
[31m-#[m
[31m-[m
[31m-newmtl leftWall[m
[31m-  Ns 10.0000[m
[31m-  Ni 1.5000[m
[31m-  illum 2[m
[31m-  Ka 0.953 0.357 0.212 # Orange[m
[31m-  Kd 0.953 0.357 0.212[m
[31m-  Ks 0 0 0[m
[31m-  Ke 0 0 0[m
[31m-[m
[31m-[m
[31m-newmtl rightWall[m
[31m-  Ns 10.0000[m
[31m-  Ni 1.5000[m
[31m-  illum 2[m
[31m-  Ka 0.486 0.631 0.663 # Cyan[m
[31m-  Kd 0.486 0.631 0.663 [m
[31m-  Ks 0 0 0[m
[31m-  Ke 0 0 0[m
[31m-  [m
[31m-newmtl floor[m
[31m-  Ns 10.0000[m
[31m-  Ni 1.0000[m
[31m-  illum 2[m
[31m-  Ka 0.725 0.71 0.68 # White[m
[31m-  Kd 0.725 0.71 0.68[m
[31m-  Ks 0 0 0[m
[31m-  Ke 0 0 0[m
[31m-[m
[31m-[m
[31m-newmtl ceiling[m
[31m-  Ns 10.0000[m
[31m-  Ni 1.0000[m
[31m-  illum 2[m
[31m-  Ka 0.725 0.71 0.68 # White[m
[31m-  Kd 0.725 0.71 0.68[m
[31m-  Ks 0     0    0[m
[31m-  Ke 0     0    0[m
[31m-  [m
[31m-[m
[31m-newmtl backWall[m
[31m-  Ns 10.0000[m
[31m-  Ni 1.0000[m
[31m-  illum 2[m
[31m-  Ka 0.725 0.71 0.68 # White[m
[31m-  Kd 0.725 0.71 0.68[m
[31m-  Ks 0 0 0[m
[31m-  Ke 0 0 0[m
[31m-[m
[31m-  newmtl light[m
[31m-	Ns 10.0000[m
[31m-	Ni 1.5000[m
[31m-	d 1.0000[m
[31m-	Tr 0.0000[m
[31m-	Tf 1.0000 1.0000 1.0000 [m
[31m-	illum 2[m
[31m-	Ka 0.7800 0.7800 0.7800[m
[31m-	Kd 0.7800 0.7800 0.7800[m
[31m-    Ke 10 10 10[m
[31m-	Ks 0 0 0[m
[31m-[m
[1mdiff --git a/Rescources/models/CornellBox/CornellBox-Empty-RG.mtl b/Rescources/models/CornellBox/CornellBox-Empty-RG.mtl[m
[1mdeleted file mode 100644[m
[1mindex aeec085..0000000[m
[1m--- a/Rescources/models/CornellBox/CornellBox-Empty-RG.mtl[m
[1m+++ /dev/null[m
[36m@@ -1,69 +0,0 @@[m
[31m-# An empty Cornell Box with red and blue walls in OBJ format.[m
[31m-# Note that the real box is not a perfect cube, so[m
[31m-# the faces are imperfect in this data set.[m
[31m-#[m
[31m-# Created by Guedis Cardenas and Morgan McGuire at Williams College, 2011[m
[31m-# Released into the Public Domain.[m
[31m-#[m
[31m-# http://graphics.cs.williams.edu/data[m
[31m-# http://www.graphics.cornell.edu/online/box/data.html[m
[31m-#[m
[31m-[m
[31m-[m
[31m-newmtl leftWall[m
[31m-  Ns 10.0000[m
[31m-  Ni 1.5000[m
[31m-  illum 2[m
[31m-  Ka 0.63 0.065 0.05 # Red[m
[31m-  Kd 0.63 0.065 0.05[m
[31m-  Ks 0 0 0[m
[31m-  Ke 0 0 0[m
[31m-[m
[31m-[m
[31m-newmtl rightWall[m
[31m-  Ns 10.0000[m
[31m-  Ni 1.5000[m
[31m-  illum 2[m
[31m-  Ka 0.14 0.45 0.091 # Green[m
[31m-  Kd 0.14 0.45 0.091[m
[31m-  Ks 0 0 0[m
[31m-  Ke 0 0 0[m
[31m-[m
[31m-  [m
[31m-newmtl floor[m
[31m-  Ns 10.0000[m
[31m-  Ni 1.0000[m
[31m-  illum 2[m
[31m-  Ka 0.725 0.71 0.68 # White[m
[31m-  Kd 0.725 0.71 0.68[m
[31m-  Ks 0 0 0[m
[31m-  Ke 0 0 0[m
[31m-[m
[31m-[m
[31m-newmtl ceiling[m
[31m-  Ns 10.0000[m
[31m-  Ni 1.0000[m
[31m-  illum 2[m
[31m-  Ka 0.725 0.71 0.68 # White[m
[31m-  Kd 0.725 0.71 0.68[m
[31m-  Ks 0     0    0[m
[31m-  Ke 0     0    0[m
[31m-  [m
[31m-[m
[31m-newmtl backWall[m
[31m-  Ns 10.0000[m
[31m-  Ni 1.0000[m
[31m-  illum 2[m
[31m-  Ka 0.725 0.71 0.68 # White[m
[31m-  Kd 0.725 0.71 0.68[m
[31m-  Ks 0 0 0[m
[31m-  Ke 0 0 0[m
[31m-[m
[31m-newmtl light[m
[31m-  Ns 10.0000[m
[31m-  Ni 1.0000[m
[31m-  illum 2[m
[31m-  Ka 0.78 0.78 0.78 # White[m
[31m-  Kd 0.78 0.78 0.78[m
[31m-  Ks 0 0 0[m
[31m-  Ke 17 12 4[m
\ No newline at end of file[m
[1mdiff --git a/Rescources/models/CornellBox/CornellBox-Empty-Squashed.mtl b/Rescources/models/CornellBox/CornellBox-Empty-Squashed.mtl[m
[1mdeleted file mode 100644[m
[1mindex 180b037..0000000[m
[1m--- a/Rescources/models/CornellBox/CornellBox-Empty-Squashed.mtl[m
[1m+++ /dev/null[m
[36m@@ -1,71 +0,0 @@[m
[31m-# The empty water Cornell Box as seen in Henrik Jensen's [m
[31m-# "Realistic Image Synthesis Using Photon Mapping" (Page 109 Fig. 9.14) in OBJ format.[m
[31m-# Note that the real box is not a perfect cube, so[m
[31m-# the faces are imperfect in this data set.[m
[31m-#[m
[31m-# Created by Guedis Cardenas and Morgan McGuire at Williams College, 2011[m
[31m-# Released into the Public Domain.[m
[31m-#[m
[31m-# http://graphics.cs.williams.edu/data[m
[31m-# http://www.graphics.cornell.edu/online/box/data.html[m
[31m-#[m
[31m-[m
[31m-newmtl floor[m
[31m-    Ns 10.0000[m
[31m-    Ni 1.5000[m
[31m-    illum 2[m
[31m-    Ka 0.7250 0.7100 0.6800[m
[31m-    Kd 0.7250 0.7100 0.6800[m
[31m-	Ks 0 0 0[m
[31m-[m
[31m-newmtl ceiling[m
[31m-    Ns 10.0000[m
[31m-    Ni 1.5000[m
[31m-    illum 2[m
[31m-    Ka 0.7250 0.7100 0.6800[m
[31m-    Kd 0.7250 0.7100 0.6800[m
[31m-    Ks 0.0000 0.0000 0.0000[m
[31m-[m
[31m-newmtl backWall[m
[31m-	Ns 10.0000[m
[31m-	Ni 1.5000[m
[31m-	illum 2[m
[31m-	Ka 0.7250 0.7100 0.6800[m
[31m-	Kd 0.7250 0.7100 0.6800[m
[31m-	Ks 0.0000 0.0000 0.0000[m
[31m-	Ke 0.0000 0.0000 0.0000[m
[31m-[m
[31m-newmtl rightWall[m
[31m-    Ns 10.0000[m
[31m-    illum 2[m
[31m-    Ka 0.161 0.133 0.427 # Blue[m
[31m-    Kd 0.161 0.133 0.427[m
[31m-	Ks 0 0 0[m
[31m-    [m
[31m-newmtl leftWall[m
[31m-	Ns 10.0000[m
[31m-	illum 2[m
[31m-	Ka 0.6300 0.0650 0.0500[m
[31m-	Kd 0.6300 0.0650 0.0500[m
[31m-	Ks 0 0 0[m
[31m-[m
[31m-newmtl light[m
[31m-	Ns 10.0000[m
[31m-	Ni 1.5000[m
[31m-	d 1.0000[m
[31m-	Tr 0.0000[m
[31m-	Tf 1.0000 1.0000 1.0000 [m
[31m-	illum 2[m
[31m-	Ka 0.7800 0.7800 0.7800[m
[31m-	Kd 0.7800 0.7800 0.7800[m
[31m-    Ke 10 10 10[m
[31m-	Ks 0 0 0[m
[31m-[m
[31m-newmtl water[m
[31m-	 Ka 0.01 0.01 0.01[m
[31m-    Kd 0.01 0.01 0.01[m
[31m-    Ks 0.30 0.30 0.30[m
[31m-    Tf 0.10 0.10 0.10[m
[31m-    Ns 200[m
[31m-    Ni 1.33[m
[31m-    illum 7[m
[1mdiff --git a/Rescources/models/CornellBox/CornellBox-Empty-White.mtl b/Rescources/models/CornellBox/CornellBox-Empty-White.mtl[m
[1mdeleted file mode 100644[m
[1mindex 88136a0..0000000[m
[1m--- a/Rescources/models/CornellBox/CornellBox-Empty-White.mtl[m
[1m+++ /dev/null[m
[36m@@ -1,71 +0,0 @@[m
[31m-# An empty Cornell Box with cyan and orange walls in OBJ format.[m
[31m-# Note that the real box is not a perfect cube, so[m
[31m-# the faces are imperfect in this data set.[m
[31m-#[m
[31m-# Created by Guedis Cardenas and Morgan McGuire at Williams College, 2011[m
[31m-# Released into the Public Domain.[m
[31m-#[m
[31m-# http://graphics.cs.williams.edu/data[m
[31m-# http://www.graphics.cornell.edu/online/box/data.html[m
[31m-#[m
[31m-[m
[31m-newmtl leftWall[m
[31m-  Ns 10.0000[m
[31m-  Ni 1.5000[m
[31m-  illum 2[m
[31m-  Ka 1 1 1 [m
[31m-  Kd 1 1 1[m
[31m-  Ks 0 0 0[m
[31m-  Ke 0 0 0[m
[31m-[m
[31m-[m
[31m-newmtl rightWall[m
[31m-  Ns 10.0000[m
[31m-  Ni 1.5000[m
[31m-  illum 2[m
[31m-  Ka 1 1 1[m
[31m-  Kd 1 1 1[m
[31m-  Ks 0 0 0[m
[31m-  Ke 0 0 0[m
[31m-  [m
[31m-newmtl floor[m
[31m-  Ns 10.0000[m
[31m-  Ni 1.0000[m
[31m-  illum 2[m
[31m-  Ka 1 1 1[m
[31m-  Kd 1 1 1[m
[31m-  Ks 0 0 0[m
[31m-  Ke 0 0 0[m
[31m-[m
[31m-[m
[31m-newmtl ceiling[m
[31m-  Ns 10.0000[m
[31m-  Ni 1.0000[m
[31m-  illum 2[m
[31m-  Ka 1 1 1[m
[31m-  Kd 1 1 1[m
[31m-  Ks 0     0    0[m
[31m-  Ke 0     0    0[m
[31m-  [m
[31m-[m
[31m-newmtl backWall[m
[31m-  Ns 10.0000[m
[31m-  Ni 1.0000[m
[31m-  illum 2[m
[31m-  Ka 1 1 1[m
[31m-  Kd 1 1 1[m
[31m-  Ks 0 0 0[m
[31m-  Ke 0 0 0[m
[31m-[m
[31m-newmtl light[m
[31m-Ns 10.0000[m
[31m-Ni 1.5000[m
[31m-d 1.0000[m
[31m-Tr 0.0000[m
[31m-Tf 1.0000 1.0000 1.0000 [m
[31m-illum 2[m
[31m-Ka 0.7800 0.7800 0.7800[m
[31m-Kd 0.7800 0.7800 0.7800[m
[31m-Ke 10 10 10[m
[31m-Ks 0 0 0[m
[31m-[m
[1mdiff --git a/Rescources/models/CornellBox/CornellBox-Glossy-Floor.mtl b/Rescources/models/CornellBox/CornellBox-Glossy-Floor.mtl[m
[1mdeleted file mode 100644[m
[1mindex f97d047..0000000[m
[1m--- a/Rescources/models/CornellBox/CornellBox-Glossy-Floor.mtl[m
[1m+++ /dev/null[m
[36m@@ -1,98 +0,0 @@[m
[31m-# 3ds Max Wavefront OBJ Exporter v0.97b - (c)2007 guruware[m
[31m-# File Created: 05.12.2012 22:05:26[m
[31m-[m
[31m-newmtl sphere[m
[31m-	Ns 32.0000[m
[31m-	Ni 1.5000[m
[31m-	d 1.0000[m
[31m-	Tr 0.0000[m
[31m-	Tf 1.0000 1.0000 1.0000 [m
[31m-	illum 2[m
[31m-	Ka 0.4860 0.6310 0.6630[m
[31m-	Kd 0.4860 0.6310 0.6630[m
[31m-	Ks 0.7000 0.7000 0.7000[m
[31m-	Ke 0.0000 0.0000 0.0000[m
[31m-[m
[31m-newmtl shortBox[m
[31m-	Ns 10.0000[m
[31m-	Ni 1.5000[m
[31m-	d 1.0000[m
[31m-	Tr 0.0000[m
[31m-	Tf 1.0000 1.0000 1.0000 [m
[31m-	illum 2[m
[31m-	Ka 0.7250 0.7100 0.6800[m
[31m-	Kd 0.7250 0.7100 0.6800[m
[31m-	Ks 0.0000 0.0000 0.0000[m
[31m-	Ke 0.0000 0.0000 0.0000[m
[31m-[m
[31m-newmtl floor[m
[31m-	Ns 32.0000[m
[31m-	Ni 1.5000[m
[31m-	d 1.0000[m
[31m-	Tr 0.0000[m
[31m-	Tf 1.0000 1.0000 1.0000 [m
[31m-	illum 2[m
[31m-	Ka 0.7250 0.7100 0.6800[m
[31m-	Kd 0.7250 0.7100 0.6800[m
[31m-	Ks 0.3000 0.3000 0.3000[m
[31m-	Ke 0.0000 0.0000 0.0000[m
[31m-[m
[31m-newmtl ceiling[m
[31m-	Ns 10.0000[m
[31m-	Ni 1.5000[m
[31m-	d 1.0000[m
[31m-	Tr 0.0000[m
[31m-	Tf 1.0000 1.0000 1.0000 [m
[31m-	illum 2[m
[31m-	Ka 0.7250 0.7100 0.6800[m
[31m-	Kd 0.7250 0.7100 0.6800[m
[31m-	Ks 0.0000 0.0000 0.0000[m
[31m-	Ke 0.0000 0.0000 0.0000[m
[31m-[m
[31m-newmtl backWall[m
[31m-	Ns 10.0000[m
[31m-	Ni 1.5000[m
[31m-	d 1.0000[m
[31m-	Tr 0.0000[m
[31m-	Tf 1.0000 1.0000 1.0000 [m
[31m-	illum 2[m
[31m-	Ka 0.7250 0.7100 0.6800[m
[31m-	Kd 0.7250 0.7100 0.6800[m
[31m-	Ks 0.0000 0.0000 0.0000[m
[31m-	Ke 0.0000 0.0000 0.0000[m
[31m-[m
[31m-newmtl rightWall[m
[31m-	Ns 10.0000[m
[31m-	Ni 1.5000[m
[31m-	d 1.0000[m
[31m-	Tr 0.0000[m
[31m-	Tf 1.0000 1.0000 1.0000 [m
[31m-	illum 2[m
[31m-	Ka 0.3725 0.6480 0.3137[m
[31m-	Kd 0.3725 0.6480 0.3137[m
[31m-	Ks 0.0000 0.0000 0.0000[m
[31m-	Ke 0.0000 0.0000 0.0000[m
[31m-[m
[31m-newmtl leftWall[m
[31m-	Ns 10.0000[m
[31m-	Ni 1.5000[m
[31m-	d 1.0000[m
[31m-	Tr 0.0000[m
[31m-	Tf 1.0000 1.0000 1.0000 [m
[31m-	illum 2[m
[31m-	Ka 0.6300 0.0650 0.0500[m
[31m-	Kd 0.6300 0.0650 0.0500[m
[31m-	Ks 0.0000 0.0000 0.0000[m
[31m-	Ke 0.0000 0.0000 0.0000[m
[31m-[m
[31m-newmtl light[m
[31m-	Ns 10.0000[m
[31m-	Ni 1.5000[m
[31m-	d 1.0000[m
[31m-	Tr 0.0000[m
[31m-	Tf 1.0000 1.0000 1.0000 [m
[31m-	illum 2[m
[31m-	Ka 0.7800 0.7800 0.7800[m
[31m-	Kd 0.7800 0.7800 0.7800[m
[31m-	Ks 0.0000 0.0000 0.0000[m
[31m-	Ke 1.0000 1.0000 1.0000[m
[1mdiff --git a/Rescources/models/CornellBox/CornellBox-Glossy.mtl b/Rescources/models/CornellBox/CornellBox-Glossy.mtl[m
[1mdeleted file mode 100644[m
[1mindex e2b092d..0000000[m
[1m--- a/Rescources/models/CornellBox/CornellBox-Glossy.mtl[m
[1m+++ /dev/null[m
[36m@@ -1,76 +0,0 @@[m
[31m-# 3ds Max Wavefront OBJ Exporter v0.97b - (c)2007 guruware[m
[31m-# File Created: 05.12.2012 22:05:26[m
[31m-[m
[31m-newmtl sphere[m
[31m-	Ns 40.0000[m
[31m-	Ni 1.5000[m
[31m-	d 1.0000[m
[31m-	Tr 0.0000[m
[31m-	Tf 1.0000 1.0000 1.0000 [m
[31m-	illum 2[m
[31m-	Ka 0.4860 0.6310 0.6630[m
[31m-	Kd 0.4860 0.6310 0.6630[m
[31m-	Ks 0.9000 0.9000 0.9000[m
[31m-	Ke 0.0000 0.0000 0.0000[m
[31m-[m
[31m-newmtl shortBox[m
[31m-	Ns 40.0000[m
[31m-	Ni 1.5000[m
[31m-	d 1.0000[m
[31m-	Tr 0.0000[m
[31m-	Tf 1.0000 1.0000 1.0000 [m
[31m-	illum 2[m
[31m-	Ka 0.5250 0.5100 0.4800[m
[31m-	Kd 0.5250 0.5100 0.4800[m
[31m-	Ks 0.8000 0.8000 0.8000[m
[31m-	Ke 0.0000 0.0000 0.0000[m
[31m-[m
[31m-[m
[31m-newmtl leftWall[m
[31m-  Ns 10.0000[m
[31m-  Ni 1.5000[m
[31m-  illum 2[m
[31m-  Ka 0.63 0.065 0.05 # Red[m
[31m-  Kd 0.63 0.065 0.05[m
[31m-  Ks 0 0 0[m
[31m-  Ke 0 0 0[m
[31m-[m
[31m-[m
[31m-newmtl rightWall[m
[31m-  Ns 10.0000[m
[31m-  Ni 1.5000[m
[31m-  illum 2[m
[31m-  Ka 0.14 0.45 0.091 # Green[m
[31m-  Kd 0.14 0.45 0.091[m
[31m-  Ks 0 0 0[m
[31m-  Ke 0 0 0[m
[31m-[m
[31m-  [m
[31m-newmtl floor[m
[31m-  Ns 10.0000[m
[31m-  Ni 1.0000[m
[31m-  illum 2[m
[31m-  Ka 0.725 0.71 0.68 # White[m
[31m-  Kd 0.725 0.71 0.68[m
[31m-  Ks 0 0 0[m
[31m-  Ke 0 0 0[m
[31m-[m
[31m-[m
[31m-newmtl ceiling[m
[31m-  Ns 10.0000[m
[31m-  Ni 1.0000[m
[31m-  illum 2[m
[31m-  Ka 0.725 0.71 0.68 # White[m
[31m-  Kd 0.725 0.71 0.68[m
[31m-  Ks 0     0    0[m
[31m-  Ke 0     0    0[m
[31m-  [m
[31m-[m
[31m-newmtl backWall[m
[31m-  Ns 10.0000[m
[31m-  Ni 1.0000[m
[31m-  illum 2[m
[31m-  Ka 0.725 0.71 0.68 # White[m
[31m-  Kd 0.725 0.71 0.68[m
[31m-  Ks 0 0 0[m
[31m-  Ke 0 0 0[m
\ No newline at end of file[m
[1mdiff --git a/Rescources/models/CornellBox/CornellBox-Mirror.mtl b/Rescources/models/CornellBox/CornellBox-Mirror.mtl[m
[1mdeleted file mode 100644[m
[1mindex 2f529cc..0000000[m
[1m--- a/Rescources/models/CornellBox/CornellBox-Mirror.mtl[m
[1m+++ /dev/null[m
[36m@@ -1,87 +0,0 @@[m
[31m-# The mirror Cornell Box in OBJ format.[m
[31m-# Note that the real box is not a perfect cube, so[m
[31m-# the faces are imperfect in this data set.[m
[31m-#[m
[31m-# Created by Guedis Cardenas and Morgan McGuire at Williams College, 2011[m
[31m-# Released into the Public Domain.[m
[31m-#[m
[31m-# http://graphics.cs.williams.edu/data[m
[31m-# http://www.graphics.cornell.edu/online/box/data.html[m
[31m-# http://www.graphics.cornell.edu/online/box/compare.html[m
[31m-#[m
[31m-[m
[31m-newmtl leftWall[m
[31m-  Ns 10.0000[m
[31m-  Ni 1.5000[m
[31m-  illum 2[m
[31m-  Ka 0.63 0.065 0.05 # Red[m
[31m-  Kd 0.63 0.065 0.05[m
[31m-  Ks 0 0 0[m
[31m-  Ke 0 0 0[m
[31m-[m
[31m-[m
[31m-newmtl rightWall[m
[31m-  Ns 10.0000[m
[31m-  Ni 1.5000[m
[31m-  illum 2[m
[31m-  Ka 0.14 0.45 0.091 # Green[m
[31m-  Kd 0.14 0.45 0.091[m
[31m-  Ks 0 0 0[m
[31m-  Ke 0 0 0[m
[31m-[m
[31m-  [m
[31m-newmtl floor[m
[31m-  Ns 10.0000[m
[31m-  Ni 1.0000[m
[31m-  illum 2[m
[31m-  Ka 0.725 0.71 0.68 # White[m
[31m-  Kd 0.725 0.71 0.68[m
[31m-  Ks 0 0 0[m
[31m-  Ke 0 0 0[m
[31m-[m
[31m-[m
[31m-newmtl ceiling[m
[31m-  Ns 10.0000[m
[31m-  Ni 1.0000[m
[31m-  illum 2[m
[31m-  Ka 0.725 0.71 0.68 # White[m
[31m-  Kd 0.725 0.71 0.68[m
[31m-  Ks 0 0 0[m
[31m-  Ke 0 0 0[m
[31m-  [m
[31m-[m
[31m-newmtl backWall[m
[31m-  Ns 10.0000[m
[31m-  Ni 1.0000[m
[31m-  illum 2[m
[31m-  Ka 0.725 0.71 0.68 # White[m
[31m-  Kd 0.725 0.71 0.68[m
[31m-  Ks 0 0 0[m
[31m-  Ke 0 0 0[m
[31m-[m
[31m-[m
[31m-newmtl shortBox[m
[31m-  Ns 10.0000[m
[31m-  Ni 1.0000[m
[31m-  illum 2[m
[31m-  Ka 0.725 0.71 0.68 # White[m
[31m-  Kd 0.725 0.71 0.68[m
[31m-  Ks 0 0 0[m
[31m-  Ke 0 0 0[m
[31m-  [m
[31m-#the mirror mtl[m
[31m-newmtl tallBox [m
[31m- Ka 0.01 0.01 0.01[m
[31m-   Kd 0.01 0.01 0.01[m
[31m-   Ks 0.950 0.95 0.95[m
[31m-   Ns 1000[m
[31m-   illum 5[m
[31m-[m
[31m-newmtl light[m
[31m-  Ns 10.0000[m
[31m-  Ni 1.0000[m
[31m-  illum 2[m
[31m-  Ka 0.78 0.78 0.78 # White[m
[31m-  Kd 0.78 0.78 0.78[m
[31m-  Ks 0 0 0[m
[31m-  Ke 17 12 4[m
[1mdiff --git a/Rescources/models/CornellBox/CornellBox-Original.mtl b/Rescources/models/CornellBox/CornellBox-Original.mtl[m
[1mdeleted file mode 100644[m
[1mindex 191bac5..0000000[m
[1m--- a/Rescources/models/CornellBox/CornellBox-Original.mtl[m
[1m+++ /dev/null[m
[36m@@ -1,88 +0,0 @@[m
[31m-# The original Cornell Box in OBJ format.[m
[31m-# Note that the real box is not a perfect cube, so[m
[31m-# the faces are imperfect in this data set.[m
[31m-#[m
[31m-# Created by Guedis Cardenas and Morgan McGuire at Williams College, 2011[m
[31m-# Released into the Public Domain.[m
[31m-#[m
[31m-# http://graphics.cs.williams.edu/data[m
[31m-# http://www.graphics.cornell.edu/online/box/data.html[m
[31m-#[m
[31m-[m
[31m-newmtl leftWall[m
[31m-  Ns 10.0000[m
[31m-  Ni 1.5000[m
[31m-  illum 2[m
[31m-  Ka 0.63 0.065 0.05 # Red[m
[31m-  Kd 0.63 0.065 0.05[m
[31m-  Ks 0 0 0[m
[31m-  Ke 0 0 0[m
[31m-[m
[31m-[m
[31m-newmtl rightWall[m
[31m-  Ns 10.0000[m
[31m-  Ni 1.5000[m
[31m-  illum 2[m
[31m-  Ka 0.14 0.45 0.091 # Green[m
[31m-  Kd 0.14 0.45 0.091[m
[31m-  Ks 0 0 0[m
[31m-  Ke 0 0 0[m
[31m-[m
[31m-  [m
[31m-newmtl floor[m
[31m-  Ns 10.0000[m
[31m-  Ni 1.0000[m
[31m-  illum 2[m
[31m-  Ka 0.725 0.71 0.68 # White[m
[31m-  Kd 0.725 0.71 0.68[m
[31m-  Ks 0 0 0[m
[31m-  Ke 0 0 0[m
[31m-[m
[31m-[m
[31m-newmtl ceiling[m
[31m-  Ns 10.0000[m
[31m-  Ni 1.0000[m
[31m-  illum 2[m
[31m-  Ka 0.725 0.71 0.68 # White[m
[31m-  Kd 0.725 0.71 0.68[m
[31m-  Ks 0     0    0[m
[31m-  Ke 0     0    0[m
[31m-  [m
[31m-[m
[31m-newmtl backWall[m
[31m-  Ns 10.0000[m
[31m-  Ni 1.0000[m
[31m-  illum 2[m
[31m-  Ka 0.725 0.71 0.68 # White[m
[31m-  Kd 0.725 0.71 0.68[m
[31m-  Ks 0 0 0[m
[31m-  Ke 0 0 0[m
[31m-[m
[31m-[m
[31m-newmtl shortBox[m
[31m-  Ns 10.0000[m
[31m-  Ni 1.0000[m
[31m-  illum 2[m
[31m-  Ka 0.725 0.71 0.68 # White[m
[31m-  Kd 0.725 0.71 0.68[m
[31m-  Ks 0 0 0[m
[31m-  Ke 0 0 0[m
[31m-  [m
[31m-[m
[31m-newmtl tallBox[m
[31m-  Ns 10.0000[m
[31m-  Ni 1.0000[m
[31m-  illum 2[m
[31m-  Ka 0.725 0.71 0.68 # White[m
[31m-  Kd 0.725 0.71 0.68[m
[31m-  Ks 0 0 0[m
[31m-  Ke 0 0 0[m
[31m-[m
[31m-newmtl light[m
[31m-  Ns 10.0000[m
[31m-  Ni 1.0000[m
[31m-  illum 2[m
[31m-  Ka 0.78 0.78 0.78 # White[m
[31m-  Kd 0.78 0.78 0.78[m
[31m-  Ks 0 0 0[m
[31m-  Ke 17 12 4[m
[1mdiff --git a/Rescources/models/CornellBox/CornellBox-Sphere.mtl b/Rescources/models/CornellBox/CornellBox-Sphere.mtl[m
[1mdeleted file mode 100644[m
[1mindex f736779..0000000[m
[1m--- a/Rescources/models/CornellBox/CornellBox-Sphere.mtl[m
[1m+++ /dev/null[m
[36m@@ -1,80 +0,0 @@[m
[31m-# The sphere Cornell Box as seen in Henrik Jensen's [m
[31m-# "Realistic Image Synthesis Using Photon Mapping" (Page 107 Fig. 9.10) in OBJ format.[m
[31m-# Note that the real box is not a perfect cube, so[m
[31m-# the faces are imperfect in this data set.[m
[31m-#[m
[31m-# Created by Guedis Cardenas and Morgan McGuire at Williams College, 2011[m
[31m-# Released into the Public Domain.[m
[31m-#[m
[31m-# http://graphics.cs.williams.edu/data[m
[31m-# http://www.graphics.cornell.edu/online/box/data.html[m
[31m-# http://www.cs.cmu.edu/~djames/15-864/pics/cornellBox.jpg[m
[31m-#[m
[31m-[m
[31m-[m
[31m-newmtl leftSphere[m
[31m-    Ka 0.01 0.01 0.01[m
[31m-    Kd 0.01 0.01 0.01[m
[31m-    Ks 0.95 0.95 0.95[m
[31m-    Ns 1024[m
[31m-    illum 5[m
[31m-    [m
[31m-newmtl rightSphere[m
[31m-    Ka 0.01 0.01 0.01[m
[31m-    Kd 0.01 0.01 0.01[m
[31m-    Ks 0.30 0.30 0.30[m
[31m-    Tf 0.10 0.10 0.10[m
[31m-    Ns 1024[m
[31m-    Ni 2.5[m
[31m-    illum 7[m
[31m-   [m
[31m-newmtl floor[m
[31m-    Ns 10.0000[m
[31m-    Ni 1.5000[m
[31m-    illum 2[m
[31m-    Ka 0.7250 0.7100 0.6800[m
[31m-    Kd 0.7250 0.7100 0.6800[m
[31m-	Ks 0 0 0[m
[31m-[m
[31m-newmtl ceiling[m
[31m-    Ns 10.0000[m
[31m-    Ni 1.5000[m
[31m-    illum 2[m
[31m-    Ka 0.7250 0.7100 0.6800[m
[31m-    Kd 0.7250 0.7100 0.6800[m
[31m-    Ks 0.0000 0.0000 0.0000[m
[31m-[m
[31m-newmtl backWall[m
[31m-	Ns 10.0000[m
[31m-	Ni 1.5000[m
[31m-	illum 2[m
[31m-	Ka 0.7250 0.7100 0.6800[m
[31m-	Kd 0.7250 0.7100 0.6800[m
[31m-	Ks 0.0000 0.0000 0.0000[m
[31m-	Ke 0.0000 0.0000 0.0000[m
[31m-[m
[31m-newmtl rightWall[m
[31m-    Ns 10.0000[m
[31m-    illum 2[m
[31m-    Ka 0.161 0.133 0.427 # Blue[m
[31m-    Kd 0.161 0.133 0.427[m
[31m-	Ks 0 0 0[m
[31m-    [m
[31m-newmtl leftWall[m
[31m-	Ns 10.0000[m
[31m-	illum 2[m
[31m-	Ka 0.6300 0.0650 0.0500[m
[31m-	Kd 0.6300 0.0650 0.0500[m
[31m-	Ks 0 0 0[m
[31m-[m
[31m-newmtl light[m
[31m-	Ns 10.0000[m
[31m-	Ni 1.5000[m
[31m-	d 1.0000[m
[31m-	Tr 0.0000[m
[31m-	Tf 1.0000 1.0000 1.0000 [m
[31m-	illum 2[m
[31m-	Ka 0.7800 0.7800 0.7800[m
[31m-	Kd 0.7800 0.7800 0.7800[m
[31m-    Ke 10 10 10[m
[31m-	Ks 0 0 0[m
[1mdiff --git a/Rescources/models/CornellBox/CornellBox-Water.mtl b/Rescources/models/CornellBox/CornellBox-Water.mtl[m
[1mdeleted file mode 100644[m
[1mindex 0a16703..0000000[m
[1m--- a/Rescources/models/CornellBox/CornellBox-Water.mtl[m
[1m+++ /dev/null[m
[36m@@ -1,86 +0,0 @@[m
[31m-# The water Cornell Box as seen in Henrik Jensen's [m
[31m-# "Realistic Image Synthesis Using Photon Mapping" (Page 109 Fig. 9.14) in OBJ format.[m
[31m-# Note that the real box is not a perfect cube, so[m
[31m-# the faces are imperfect in this data set.[m
[31m-#[m
[31m-# Created by Guedis Cardenas and Morgan McGuire at Williams College, 2011[m
[31m-# Released into the Public Domain.[m
[31m-#[m
[31m-# http://graphics.cs.williams.edu/data[m
[31m-# http://www.graphics.cornell.edu/online/box/data.html[m
[31m-#[m
[31m-newmtl leftSphere[m
[31m-    Ka 0.01 0.01 0.01[m
[31m-    Kd 0.01 0.01 0.01[m
[31m-    Ks 0.95 0.95 0.95[m
[31m-    Ns 1000[m
[31m-    illum 5[m
[31m-    [m
[31m-newmtl rightSphere[m
[31m-    Ka 0.01 0.01 0.01[m
[31m-    Kd 0.01 0.01 0.01[m
[31m-    Ks 0.30 0.30 0.30[m
[31m-    Tf 0.10 0.10 0.10[m
[31m-    Ns 200[m
[31m-    Ni 2.5[m
[31m-    illum 7[m
[31m-   [m
[31m-newmtl floor[m
[31m-    Ns 10.0000[m
[31m-    Ni 1.5000[m
[31m-    illum 2[m
[31m-    Ka 0.7250 0.7100 0.6800[m
[31m-    Kd 0.7250 0.7100 0.6800[m
[31m-	Ks 0 0 0[m
[31m-[m
[31m-newmtl ceiling[m
[31m-    Ns 10.0000[m
[31m-    Ni 1.5000[m
[31m-    illum 2[m
[31m-    Ka 0.7250 0.7100 0.6800[m
[31m-    Kd 0.7250 0.7100 0.6800[m
[31m-    Ks 0.0000 0.0000 0.0000[m
[31m-[m
[31m-newmtl backWall[m
[31m-	Ns 10.0000[m
[31m-	Ni 1.5000[m
[31m-	illum 2[m
[31m-	Ka 0.7250 0.7100 0.6800[m
[31m-	Kd 0.7250 0.7100 0.6800[m
[31m-	Ks 0.0000 0.0000 0.0000[m
[31m-	Ke 0.0000 0.0000 0.0000[m
[31m-[m
[31m-newmtl rightWall[m
[31m-    Ns 10.0000[m
[31m-    illum 2[m
[31m-    Ka 0.161 0.133 0.427 # Blue[m
[31m-    Kd 0.161 0.133 0.427[m
[31m-	Ks 0 0 0[m
[31m-    [m
[31m-newmtl leftWall[m
[31m-	Ns 10.0000[m
[31m-	illum 2[m
[31m-	Ka 0.6300 0.0650 0.0500[m
[31m-	Kd 0.6300 0.0650 0.0500[m
[31m-	Ks 0 0 0[m
[31m-[m
[31m-newmtl light[m
[31m-	Ns 10.0000[m
[31m-	Ni 1.5000[m
[31m-	d 1.0000[m
[31m-	Tr 0.0000[m
[31m-	Tf 1.0000 1.0000 1.0000 [m
[31m-	illum 2[m
[31m-	Ka 0.7800 0.7800 0.7800[m
[31m-	Kd 0.7800 0.7800 0.7800[m
[31m-    Ke 10 10 10[m
[31m-	Ks 0 0 0[m
[31m-[m
[31m-newmtl water[m
[31m-	 Ka 0.01 0.01 0.01[m
[31m-    Kd 0.01 0.01 0.01[m
[31m-    Ks 0.30 0.30 0.30[m
[31m-    Tf 0.10 0.10 0.10[m
[31m-    Ns 200[m
[31m-    Ni 1.33[m
[31m-    illum 7[m
[1mdiff --git a/Rescources/models/CornellBox/copyright.txt b/Rescources/models/CornellBox/copyright.txt[m
[1mdeleted file mode 100644[m
[1mindex dc04c0d..0000000[m
[1m--- a/Rescources/models/CornellBox/copyright.txt[m
[1m+++ /dev/null[m
[36m@@ -1,12 +0,0 @@[m
[31m-# A set of eight Cornell Boxes in OBJ format.[m
[31m-# Note that the real box is not a perfect cube, so[m
[31m-# the faces are imperfect in this data set.[m
[31m-#[m
[31m-# Created by Guedis Cardenas and Morgan McGuire at Williams College, 2011[m
[31m-# Attribution 3.0 Unported (CC BY 3.0) License[m
[31m-# http://creativecommons.org/licenses/by/3.0/[m
[31m-#[m
[31m-# http://graphics.cs.williams.edu/data[m
[31m-# [m
[31m-[m
[31m-[m
[1mdiff --git a/Rescources/models/CornellBox/water.mtl b/Rescources/models/CornellBox/water.mtl[m
[1mdeleted file mode 100644[m
[1mindex 4c11dab..0000000[m
[1m--- a/Rescources/models/CornellBox/water.mtl[m
[1m+++ /dev/null[m
[36m@@ -1,11 +0,0 @@[m
[31m-# Water for the Cornell Box as seen in Henrik Jensen's [m
[31m-# "Realistic Image Synthesis Using Photon Mapping" (Page 109 Fig. 9.14) in OBJ format.[m
[31m-[m
[31m-newmtl water[m
[31m-	Ka 0.01 0.01 0.01[m
[31m-    Kd 0.01 0.01 0.01[m
[31m-    Ks 0.30 0.30 0.30[m
[31m-    Tf 0.10 0.10 0.10[m
[31m-    Ns 200[m
[31m-    Ni 1.33[m
[31m-    illum 7[m
[1mdiff --git a/Rescources/models/San_Miguel/license.txt b/Rescources/models/San_Miguel/license.txt[m
[1mdeleted file mode 100644[m
[1mindex 0dd37d1..0000000[m
[1m--- a/Rescources/models/San_Miguel/license.txt[m
[1m+++ /dev/null[m
[36m@@ -1,34 +0,0 @@[m
[31m-San Miguel 2.1[m
[31m-[m
[31m-Known bugs (to be fixed soon): [m
[31m-[m
[31m-- Some bump and normal maps are not correctly mapped in the MTL[m
[31m-- Some surfaces are too glossy[m
[31m-- No refraction on glass and water[m
[31m-- Reflection maps are low dynamic range[m
[31m-[m
[31m----------------------[m
[31m-[m
[31m-San Miguel, modelled by and copyright Guillermo M. Leal Llaguno (http://www.evvisual.com/)[m
[31m-[m
[31m-2017 version improved by Morgan McGuire, Guedis Cardenas, and Michael Mara at Williams College[m
[31m-and Nicholas Hull at NVIDIA with permission from Mr. Llaguno.[m
[31m-[m
[31m-This model is free for research and educational use with attribution.[m
[31m-[m
[31m-http://casual-effects.com/data[m
[31m-[m
[31m-________________________________________________________[m
[31m-[m
[31m-This scene was modeled by Guillermo M. Leal Llaguno of Evolucien Visual, [m
[31m-based on a hacienda that he visited in San Miguel de Allende, Mexico. [m
[31m-[m
[31m-We began with Guillermo's original 3DS Max file and flattened the instancing[m
[31m-from the original for convenience in research use. We then exported it as an[m
[31m-OBJ file, corrected the geometry and materials, converted materials to PNG[m
[31m-and integrated the alpha mattes, and constructed a low-polygon [m
[31m-version.[m
[31m-[m
[31m-For example, on the cover of the PBRT2 book, you can see that there[m
[31m-are chairs in the fountain and no legs on the tables! We fixed these[m
[31m-and many more problems.[m
[1mdiff --git a/Rescources/models/San_Miguel/san-miguel-low-poly.mtl b/Rescources/models/San_Miguel/san-miguel-low-poly.mtl[m
[1mdeleted file mode 100644[m
[1mindex d3fc06e..0000000[m
[1m--- a/Rescources/models/San_Miguel/san-miguel-low-poly.mtl[m
[1m+++ /dev/null[m
[36m@@ -1,2574 +0,0 @@[m
[31m-# MTL written from san-miguel-low-poly.obj[m
[31m-[m
[31m-newmtl CafeChair_Metal[m
[31m-Kd 0.48 0.48 0.48[m
[31m-Ns 256[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-[m
[31m-newmtl Default[m
[31m-Kd 0.48 0.48 0.48[m
[31m-Ns 256[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-[m
[31m-newmtl material_032[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0 0 0[m
[31m-map_Kd textures\individual_b.png[m
[31m-[m
[31m-newmtl material_033[m
[31m-Kd 1 1 1[m
[31m-Ns 256[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.196078 0.196078 0.196078[m
[31m-map_Kd textures\cenicero.png[m
[31m-[m
[31m-newmtl material_034[m
[31m-Kd 1 1 1[m
[31m-Ns 256[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.196078 0.196078 0.196078[m
[31m-map_Kd textures\ceramic_tile.png[m
[31m-[m
[31m-newmtl material_038[m
[31m-Kd 0.086274 0.082353 0.07451[m
[31m-Ns 256[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.449019 0.403922 0.354902[m
[31m-[m
[31m-newmtl material_040[m
[31m-Kd 0.30195 0.875973 0.726676[m
[31m-Ns 256[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.30195 0.875973 0.726676[m
[31m-[m
[31m-newmtl material_041[m
[31m-Kd 1 1 1[m
[31m-Ns 256[m
[31m-d 0.5[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 1 1 1[m
[31m-[m
[31m-newmtl material_042[m
[31m-Kd 0.142338 0.462081 0.235328[m
[31m-Ns 256[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.142338 0.462081 0.235328[m
[31m-[m
[31m-newmtl material_0[m
[31m-Kd 0.1 0.1 0.1[m
[31m-Ns 4096[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.5 0.5 0.5[m
[31m-[m
[31m-newmtl material_1[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\jardinera_1_color.png[m
[31m-map_Bump textures\N_jardinera_1_displacement_2.png[m
[31m-[m
[31m-newmtl material_10[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\losa.png[m
[31m-[m
[31m-newmtl material_11[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\moldura2piso_color.png[m
[31m-map_Bump textures\N_moldura2piso_bump.png[m
[31m-[m
[31m-newmtl material_12[m
[31m-Kd 0.75 0.75 0.75[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-[m
[31m-newmtl material_127[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\Maceta_B2_Color.png[m
[31m-map_Bump textures\N_Maceta_B_Bump.png[m
[31m-[m
[31m-newmtl material_128[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\Maceta_C_Color.png[m
[31m-map_Bump textures\N_Maceta_C_Bump.png[m
[31m-[m
[31m-newmtl material_129[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\Maceta_C2_Color.png[m
[31m-map_Bump textures\N_Maceta_C_Bump.png[m
[31m-[m
[31m-newmtl material_13[m
[31m-Kd 0.54902 0.54902 0.54902[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-[m
[31m-newmtl material_130[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\Maceta_D2_Color_0.png[m
[31m-map_Bump textures\N_Maceta_D_Bump_0.png[m
[31m-[m
[31m-newmtl material_131[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\dracaena_fragrans_leaf.png[m
[31m-[m
[31m-newmtl material_132[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\BS01brk.png[m
[31m-[m
[31m-newmtl material_133[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\BS01lef.png[m
[31m-[m
[31m-newmtl material_134[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\BS01flo1.png[m
[31m-[m
[31m-newmtl material_135[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\BS01flo2.png[m
[31m-[m
[31m-newmtl material_136[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\aglaonema_leaf.png[m
[31m-[m
[31m-newmtl material_137[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\aglaonema_bark.png[m
[31m-[m
[31m-newmtl material_138[m
[31m-Kd 0.147313 0.165899 0.988525[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.00589252 0.00663596 0.039541[m
[31m-map_Kd textures\aglaonema_leaf.png[m
[31m-[m
[31m-newmtl material_139[m
[31m-Kd 0.445692 0.119083 0.004669[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.0178277 0.00476332 0.00018676[m
[31m-map_Kd textures\FL13pet1.png[m
[31m-[m
[31m-newmtl material_14[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\piso_rustico.png[m
[31m-map_Ks textures\piso_rustico_Spec.png[m
[31m-map_Bump textures\N_piso_rustico_displace2.png[m
[31m-[m
[31m-newmtl material_140[m
[31m-Kd 0.008911 0.37788 0.531663[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.00035644 0.0151152 0.0212665[m
[31m-[m
[31m-newmtl material_141[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\FL16stm.png[m
[31m-[m
[31m-newmtl material_142[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\FL16lef1.png[m
[31m-[m
[31m-newmtl material_143[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\FL16pet1.png[m
[31m-[m
[31m-newmtl material_144[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\FL16lef3.png[m
[31m-[m
[31m-newmtl material_145[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\TR14lef1.png[m
[31m-[m
[31m-newmtl material_146[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\ampelopsis_brevipedunculata_bark.png[m
[31m-[m
[31m-newmtl material_147[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\HP19stm1.png[m
[31m-[m
[31m-newmtl material_148[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\HP19stm2.png[m
[31m-[m
[31m-newmtl material_149[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\HP19lef2.png[m
[31m-[m
[31m-newmtl material_15[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.00454904 0.00454904 0.00454904[m
[31m-map_Kd textures\madera_triplay_01.png[m
[31m-[m
[31m-newmtl material_150[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\HP19lef3.png[m
[31m-[m
[31m-newmtl material_151[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\HP19lef2.png[m
[31m-[m
[31m-newmtl material_152[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\FL15lef1.png[m
[31m-[m
[31m-newmtl material_153[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\FL15cnt2.png[m
[31m-[m
[31m-newmtl material_154[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\FL15stm.png[m
[31m-[m
[31m-newmtl material_155[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\FL15pet2.png[m
[31m-[m
[31m-newmtl material_156[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\FL15lef2.png[m
[31m-[m
[31m-newmtl material_157[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\FL15cnt1.png[m
[31m-[m
[31m-newmtl material_158[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\FL15lef1.png[m
[31m-[m
[31m-newmtl material_159[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\FL15lef2.png[m
[31m-[m
[31m-newmtl material_16[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0 0 0[m
[31m-map_Kd textures\rust_a1.png[m
[31m-[m
[31m-newmtl material_160[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\FL15pet2.png[m
[31m-[m
[31m-newmtl material_161[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\HP07lef1.png[m
[31m-[m
[31m-newmtl material_162[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\HP07stm1.png[m
[31m-[m
[31m-newmtl material_163[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\HP07lef2.png[m
[31m-[m
[31m-newmtl material_164[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\HP07stm2.png[m
[31m-[m
[31m-newmtl material_165[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\FL11pet1.png[m
[31m-[m
[31m-newmtl material_166[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\FL11pet2.png[m
[31m-[m
[31m-newmtl material_167[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\FL11sta.png[m
[31m-[m
[31m-newmtl material_168[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\FL11pet3.png[m
[31m-[m
[31m-newmtl material_169[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\FL11pet4.png[m
[31m-[m
[31m-newmtl material_17[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\muros_a.png[m
[31m-[m
[31m-newmtl material_170[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\FL11lef1.png[m
[31m-[m
[31m-newmtl material_171[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\FL11lef2.png[m
[31m-[m
[31m-newmtl material_172[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\FL11stm.png[m
[31m-[m
[31m-newmtl material_173[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\FL11pet1.png[m
[31m-[m
[31m-newmtl material_174[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\FL11pet2.png[m
[31m-[m
[31m-newmtl material_175[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\FL11pet3.png[m
[31m-[m
[31m-newmtl material_176[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\FL11pet4.png[m
[31m-[m
[31m-newmtl material_177[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\FL11lef1.png[m
[31m-[m
[31m-newmtl material_178[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\HP17lef1.png[m
[31m-[m
[31m-newmtl material_179[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\HP17lef2.png[m
[31m-[m
[31m-newmtl material_18[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\muros_b1.png[m
[31m-map_Bump textures\N_muros_b1.png[m
[31m-[m
[31m-newmtl material_180[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\HP17stm.png[m
[31m-[m
[31m-newmtl material_181[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\HP11stm.png[m
[31m-map_Bump textures\N_HP11stm.png[m
[31m-[m
[31m-newmtl material_182[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.00470588 0.00470588 0.00470588[m
[31m-map_Kd textures\HP11lef2.png[m
[31m-map_Bump textures\N_hp13lef1_bump.png[m
[31m-[m
[31m-newmtl material_183[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\FL13pet1.png[m
[31m-[m
[31m-newmtl material_184[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\FL13pet2.png[m
[31m-[m
[31m-newmtl material_185[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\FL13pet3.png[m
[31m-[m
[31m-newmtl material_186[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\FL13stm.png[m
[31m-[m
[31m-newmtl material_187[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\FL13lef5.png[m
[31m-[m
[31m-newmtl material_188[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\FL13lef4.png[m
[31m-[m
[31m-newmtl material_189[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\FL13lef1.png[m
[31m-[m
[31m-newmtl material_19[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\muros_c2.png[m
[31m-[m
[31m-newmtl material_190[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\elaeagnus_umbellate_bark.png[m
[31m-[m
[31m-newmtl material_191[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\impatiens_newguinea_hybrids_petal.png[m
[31m-[m
[31m-newmtl material_192[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\citrus_limon_leaf.png[m
[31m-[m
[31m-newmtl material_193[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\HP13stm2.png[m
[31m-[m
[31m-newmtl material_194[m
[31m-Kd 0.571184 0.601764 0.607166[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.0228474 0.0240706 0.0242866[m
[31m-map_Kd textures\FL13lef1.png[m
[31m-newmtl material_195[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\HP13lef2.png[m
[31m-[m
[31m-newmtl material_196[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\HP13stm2.png[m
[31m-[m
[31m-newmtl material_197[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\HP13lef3.png[m
[31m-[m
[31m-newmtl material_198[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\BS04lef.png[m
[31m-[m
[31m-newmtl material_199[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\BS04flo.png[m
[31m-[m
[31m-newmtl material_2[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\cantera_naranja_liso.png[m
[31m-[m
[31m-newmtl material_20[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\muros_f.png[m
[31m-[m
[31m-newmtl material_200[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\BS04brk.png[m
[31m-[m
[31m-newmtl material_201[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\FL12lef1.png[m
[31m-[m
[31m-newmtl material_202[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\FL12pet2.png[m
[31m-[m
[31m-newmtl material_203[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\FL12pis1.png[m
[31m-[m
[31m-newmtl material_204[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\FL12sta1.png[m
[31m-[m
[31m-newmtl material_205[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\FL12stm.png[m
[31m-[m
[31m-newmtl material_206[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\FL12pis1.png[m
[31m-[m
[31m-newmtl material_207[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\FL12pet1.png[m
[31m-[m
[31m-newmtl material_208[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\FL12stm.png[m
[31m-[m
[31m-newmtl material_209[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\HP01stm1.png[m
[31m-[m
[31m-newmtl material_21[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\muros_e.png[m
[31m-[m
[31m-newmtl material_210[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\HP01stm2.png[m
[31m-[m
[31m-newmtl material_211[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\HP01stm1.png[m
[31m-[m
[31m-newmtl material_212[m
[31m-Kd 0.340958 0.397245 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures/FL16pet1.png[m
[31m-[m
[31m-newmtl material_213[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\HP01stm1.png[m
[31m-[m
[31m-newmtl material_214[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\HP01stm2.png[m
[31m-[m
[31m-newmtl material_215[m
[31m-Kd 0.858824 0.803922 0.141176[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\FL17lef6.png[m
[31m-[m
[31m-newmtl material_216[m
[31m-Kd 1 1 1[m
[31m-Ns 16[m
[31m-d 1[m
[31m-illum 2[m
[31m-Ka 0 0 0[m
[31m-Ks 0.04 0.04 0.04[m
[31m-map_Kd textures\