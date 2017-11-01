# kinectDepthRecorder
record kinectV2 depth images (16bit tif image) and can play them later using https://github.com/RyoheiKomiyama/kinectDepthPlayer

## OS
mac

## just use this app
```
git clone https://github.com/RyoheiKomiyama/kinectDepthRecorder.git
```
and then, double click bin/kinectDepthRecorder

- 'r' key
record 16bit depth images. images are saved in bin/out/timestamp directory. you can change directory by editing settings.json
- 'p' key
togle view: depth image <-> pointcloud

## develop
this code is developed using openframeworks.
