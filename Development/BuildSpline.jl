using DelimitedFiles

UTC = readlines("UTC.42");
PosN = readdlm("PosN.42");
VelN = readdlm("VelN.42");

PosN ./= 1000.0;
VelN ./= 1000.0;


SplineFile = open("OrbSpline.txt","w");
for i=1:length(UTC)
   println(SplineFile,UTC[i].*" $(PosN[i,1]) $(PosN[i,2]) $(PosN[i,3]) $(VelN[i,1]) $(VelN[i,2]) $(VelN[i,3])");
end

close(SplineFile);

