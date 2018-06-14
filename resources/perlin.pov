camera {
   location  <0, 0, 15>
   up        <0,  1,  0>
   right     <1.33333, 0,  0>
   look_at   <0, 0, 0>
}

light_source {<-100, 100, 100> color rgb <1.5, 1.5, 1.5>}

sphere { <0, 0, 0>, 8
   pigment { color rgbf <1.0, 1.0, 1.0, 0.5> }
   finish { ambient 0.02 diffuse 0.4 specular 0.6 roughness 0.02 refraction 1.0 ior 1.33 }
}

sphere{ <0, -2, 0>, 4
   pigment { color rgbf <1.0, 1.0, 1.0, 0.5> }
   finish { ambient 0.02 diffuse 0.4 specular 0.6 roughness 0.02 refraction 1.0 ior 1.33 }
}

sphere{ <0, 2, 0>, 4
   pigment { color rgbf <1.0, 1.0, 1.0, 0.5> }
   finish { ambient 0.02 diffuse 0.4 specular 0.6 roughness 0.02 refraction 1.0 ior 1.33 }
}

sphere{ <-2, 0, 0>, 4
   pigment { color rgbf <1.0, 1.0, 1.0, 0.5> }
   finish { ambient 0.02 diffuse 0.4 specular 0.6 roughness 0.02 refraction 1.0 ior 1.33 }
}

sphere{ <2, 0, 0>, 4
   pigment { color rgbf <1.0, 1.0, 1.0, 0.5> }
   finish { ambient 0.02 diffuse 0.4 specular 0.6 roughness 0.02 refraction 1.0 ior 1.33 }
}

