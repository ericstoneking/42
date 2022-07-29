### A Pluto.jl notebook ###
# v0.14.0

using Markdown
using InteractiveUtils

# ╔═╡ 3521912c-ab93-11ec-1434-33c7441211e4
md"""
# Demonstration of Calling C from Julia
Eric Stoneking, Mar 2022

Julia has the capability to call C code (also Fortran) fairly easily.  This note uses examples from 42.

First, the desired C code must be compiled into a shared library.  The source code must be compiled with the "-fpic" option, and linked with the "-shared" option.  By custom, shared libraries have the extension ".so".
"""

# ╔═╡ 6198c672-ab94-11ec-1ce8-fb3e824ec023
libfilename = "../Kit/42kit.so"


# ╔═╡ 61370a82-ab87-11ec-3ba3-cd8fadb2ab59
Year = 2021;
Month = 8;
Day = 12;
Hour = 0;
Minute = 0;
Second = 0.0;
JD = ccall((:DateToJD,libfilename),Cdouble,(Clong,Clong,Clong,Clong,Clong,Cdouble),
    Year,Month,Day,Hour,Minute,Second)


# ╔═╡ 16201f8a-ab88-11ec-3b50-75a5bc64f13f
JD = JD+1.5
# Use collect to make 1-element arrays of the correct datatype.
Year = collect(Clong,0)
Month = collect(Clong,0)
Day = collect(Clong,0)
Hour = collect(Clong,0)
Minute = collect(Clong,0)
Second = collect(Cdouble,0.0);

ccall((:JDToDate,libfilename),Cvoid,(Cdouble,Ptr{Clong},Ptr{Clong},Ptr{Clong},Ptr{Clong},Ptr{Clong},Ptr{Cdouble}),
    JD,Year,Month,Day,Hour,Minute,Second)

# Unpack the 1-element arrays into scalars
Year = Year[1]
Month = Month[1]
Day = Day[1]
Hour = Hour[1]
Minute = Minute[1]
Second = Second[1]
(JD,Year,Month,Day,Hour,Minute,Second)


# ╔═╡ 5d820846-ab88-11ec-328d-bbe2598a5dd1
SqrtHalf = sqrt(0.5);
qx = [SqrtHalf;0;0;SqrtHalf];
qy = [0;SqrtHalf;0;SqrtHalf];
qz = [0;0;SqrtHalf;SqrtHalf];


# ╔═╡ 7d607664-ab88-11ec-00bd-4371120d2d6f
qyz = collect(Cdouble,[0.0;0.0;0.0;1.0])
ccall((:QxQ,libfilename),Cvoid,(Ptr{Cdouble},Ptr{Cdouble},Ptr{Cdouble}),qy,qz,qyz)
qyz


# ╔═╡ 87777514-ab88-11ec-079e-11db26cba1d1
qzx = collect(Cdouble,[0.0;0.0;0.0;1.0])
ccall((:QxQ,libfilename),Cvoid,(Ptr{Cdouble},Ptr{Cdouble},Ptr{Cdouble}),qz,qx,qzx)
qzx


# ╔═╡ 981c1a32-ab88-11ec-1536-79e0a66090be
qxy = collect(Cdouble,[0.0;0.0;0.0;1.0])
ccall((:QxQ,libfilename),Cvoid,(Ptr{Cdouble},Ptr{Cdouble},Ptr{Cdouble}),qx,qy,qxy)
qxy


# ╔═╡ a3d8299c-ab88-11ec-2e85-d5b657cb30df
A = [1 2 3;4 5 6;7 8 9];
CA = collect(Cdouble,A)


# ╔═╡ b0e94816-ab88-11ec-13dc-1d9cb1a2d943
b = [5;6;3];
cb = collect(Cdouble,b)


# ╔═╡ bec4eede-ab88-11ec-042f-75b92c08fbf4
A*b


# ╔═╡ c1d165d0-ab88-11ec-0d1a-679bda37007e
# Watch out for matrix transposes!  C is row-major, julia is column-major.
pc = collect(Cdouble,[0;0;0])
ccall((:MxV,libfilename),Cvoid,(Ptr{Cdouble},Ptr{Cdouble},Ptr{Cdouble}),CA,cb,pc)
pc


# ╔═╡ cae2470c-ab88-11ec-0054-571b4cb0d428
# Trying again, with the matrix transpose in the C function.
pc = collect(Cdouble,[0;0;0])
ccall((:MTxV,libfilename),Cvoid,(Ptr{Cdouble},Ptr{Cdouble},Ptr{Cdouble}),CA,cb,pc)
pc


# ╔═╡ Cell order:
# ╟─3521912c-ab93-11ec-1434-33c7441211e4
# ╠═6198c672-ab94-11ec-1ce8-fb3e824ec023
# ╠═61370a82-ab87-11ec-3ba3-cd8fadb2ab59
# ╠═16201f8a-ab88-11ec-3b50-75a5bc64f13f
# ╠═5d820846-ab88-11ec-328d-bbe2598a5dd1
# ╠═7d607664-ab88-11ec-00bd-4371120d2d6f
# ╠═87777514-ab88-11ec-079e-11db26cba1d1
# ╠═981c1a32-ab88-11ec-1536-79e0a66090be
# ╠═a3d8299c-ab88-11ec-2e85-d5b657cb30df
# ╠═b0e94816-ab88-11ec-13dc-1d9cb1a2d943
# ╠═bec4eede-ab88-11ec-042f-75b92c08fbf4
# ╠═c1d165d0-ab88-11ec-0d1a-679bda37007e
# ╠═cae2470c-ab88-11ec-0054-571b4cb0d428
