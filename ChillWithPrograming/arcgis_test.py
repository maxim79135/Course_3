import arcgis
from arcgis.widgets import MapView
from arcgis.gis import GIS

gis = GIS()
hello_map = gis.map("Москва", zoomlevel = 15)
hello_map.draw(shape='rectangle')