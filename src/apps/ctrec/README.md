# ctrec: feature tracker
 
C++ application used to track echo motion using a cross-correlation technique
to match pattern movement from one image to the next in time. Input is radar 
reflectivity, or a similar field (such as satellite IR) in MDV format. Output
is U/V fields in MDV format, as well as a copy of the  original tracked field.
Previously in RAL-CVS/apps/trec/src/ctrec
 
RAL Dependencies: dsdata, Spdb, Mdv, Radx, Ncxx, dsserver, rapformats, didss, euclid, rapmath, tdrp, toolsa, dataport