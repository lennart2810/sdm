% Simscape(TM) Multibody(TM) version: 7.4

% This is a model data file derived from a Simscape Multibody Import XML file using the smimport function.
% The data in this file sets the block parameter values in an imported Simscape Multibody model.
% For more information on this file, see the smimport function help page in the Simscape Multibody documentation.
% You can modify numerical values, but avoid any other changes to this file.
% Do not add code to this file. Do not edit the physical units shown in comments.

%%%VariableName:smiData


%============= RigidTransform =============%

%Initialize the RigidTransform structure array by filling in null values.
smiData.RigidTransform(5).translation = [0.0 0.0 0.0];
smiData.RigidTransform(5).angle = 0.0;
smiData.RigidTransform(5).axis = [0.0 0.0 0.0];
smiData.RigidTransform(5).ID = '';

%Translation Method - Cartesian
%Rotation Method - Arbitrary Axis
smiData.RigidTransform(1).translation = [0 0 0];  % mm
smiData.RigidTransform(1).angle = 3.1415926535897931;  % rad
smiData.RigidTransform(1).axis = [1 0 0];
smiData.RigidTransform(1).ID = 'B[pendulum_assembly:2:-:cart_assembly:2]';

%Translation Method - Cartesian
%Rotation Method - Arbitrary Axis
smiData.RigidTransform(2).translation = [-24.99949999999987 63.499999999999872 43.999999999999808];  % mm
smiData.RigidTransform(2).angle = 1.1662523443367929e-15;  % rad
smiData.RigidTransform(2).axis = [0.05008525673026943 0.99874494594879581 2.9169368900403472e-17];
smiData.RigidTransform(2).ID = 'F[pendulum_assembly:2:-:cart_assembly:2]';

%Translation Method - Cartesian
%Rotation Method - Arbitrary Axis
smiData.RigidTransform(3).translation = [0 11.999999999999996 10.199999999999999];  % mm
smiData.RigidTransform(3).angle = 2.0943951023931953;  % rad
smiData.RigidTransform(3).axis = [-0.57735026918962584 -0.57735026918962584 0.57735026918962584];
smiData.RigidTransform(3).ID = 'B[cart_assembly:2:-:base_assembly:2]';

%Translation Method - Cartesian
%Rotation Method - Arbitrary Axis
smiData.RigidTransform(4).translation = [-42.819452486338015 35.49999999999995 10.59999999999998];  % mm
smiData.RigidTransform(4).angle = 2.0943951023931962;  % rad
smiData.RigidTransform(4).axis = [-0.57735026918962595 -0.57735026918962606 0.5773502691896254];
smiData.RigidTransform(4).ID = 'F[cart_assembly:2:-:base_assembly:2]';

%Translation Method - Cartesian
%Rotation Method - Arbitrary Axis
smiData.RigidTransform(5).translation = [0 5.4073002406215762e-14 0];  % mm
smiData.RigidTransform(5).angle = 2.9998255056255932e-16;  % rad
smiData.RigidTransform(5).axis = [0.41961884455477561 -0.50870530697165128 -0.75175723205925538];
smiData.RigidTransform(5).ID = 'RootGround[base_assembly:2]';


%============= Solid =============%
%Center of Mass (CoM) %Moments of Inertia (MoI) %Product of Inertia (PoI)

%Initialize the Solid structure array by filling in null values.
smiData.Solid(3).mass = 0.0;
smiData.Solid(3).CoM = [0.0 0.0 0.0];
smiData.Solid(3).MoI = [0.0 0.0 0.0];
smiData.Solid(3).PoI = [0.0 0.0 0.0];
smiData.Solid(3).color = [0.0 0.0 0.0];
smiData.Solid(3).opacity = 0.0;
smiData.Solid(3).ID = '';

%Inertia Type - Custom (base)
%Visual Properties - Simple
smiData.Solid(1).mass = 2.1250599096912071;  % lbm
smiData.Solid(1).CoM = [-142.34739198394419 30.463341992790568 -53.966170328021327];  % mm
smiData.Solid(1).MoI = [4508.9457262149526 117144.32237989163 114067.33411246495];  % lbm*mm^2
smiData.Solid(1).PoI = [149.79967286264366 -9432.4829101720534 397.52302619748127];  % lbm*mm^2
%smiData.Solid(1).color = [0.74901960784313726 0.74901960784313726 0.74901960784313726];
smiData.Solid(1).color = [0.1 0.2 0.3];
smiData.Solid(1).opacity = 1;
smiData.Solid(1).ID = 'base_assembly.ipt_{0BFB5E2E-4363-5E1F-B54C-F3858DAA45B3}';

%Inertia Type - Custom (cart)
%Visual Properties - Simple
smiData.Solid(2).mass = 0.53243370429304182;  % lbm
smiData.Solid(2).CoM = [-24.980244038321466 48.187958017999421 -1.1422806039376483];  % mm
smiData.Solid(2).MoI = [492.35127273038006 301.58751775948656 494.54606496903517];  % lbm*mm^2
smiData.Solid(2).PoI = [20.106651391350933 -0.018581864062230904 0.34006812276901865];  % lbm*mm^2
%smiData.Solid(2).color = [0.74901960784313726 0.74901960784313726 0.74901960784313726];
smiData.Solid(2).color = [0.6 0.7 0.8];
smiData.Solid(2).opacity = 1;
smiData.Solid(2).ID = 'cart_assembly.ipt_{9E5D37EE-4824-1A76-F9DD-C788C7C5793B}';

%Inertia Type - Custom (pendulum)
%Visual Properties - Simple
%smiData.Solid(3).mass = 0.036083437414572123;  % lbm
smiData.Solid(3).mass = 0.56083437414572123;  % lbm
smiData.Solid(3).CoM = [8.242411302158555e-12 -258.00526508185595 4.9947026123955389];  % mm
smiData.Solid(3).MoI = [1229.6580636249182 2.1923776861360063 1231.461034605662];  % lbm*mm^2
smiData.Solid(3).PoI = [0.049178872905364233 6.6946138035097721e-10 -1.8364373187829203e-11];  % lbm*mm^2
%smiData.Solid(3).color = [0.74901960784313726 0.74901960784313726 0.74901960784313726];
smiData.Solid(3).color = [0.1 0.3 0.9];
smiData.Solid(3).opacity = 1;
smiData.Solid(3).ID = 'pendulum_assembly.ipt_{70B5C848-4C76-283B-D699-D48032241A7F}';


%============= Joint =============%
%X Revolute Primitive (Rx) %Y Revolute Primitive (Ry) %Z Revolute Primitive (Rz)
%X Prismatic Primitive (Px) %Y Prismatic Primitive (Py) %Z Prismatic Primitive (Pz) %Spherical Primitive (S)
%Constant Velocity Primitive (CV) %Lead Screw Primitive (LS)
%Position Target (Pos)

%Initialize the PrismaticJoint structure array by filling in null values.
smiData.PrismaticJoint(1).Pz.Pos = 0.0;
smiData.PrismaticJoint(1).ID = '';

smiData.PrismaticJoint(1).Pz.Pos = 0;  % m
smiData.PrismaticJoint(1).ID = '[cart_assembly:2:-:base_assembly:2]';


%Initialize the RevoluteJoint structure array by filling in null values.
smiData.RevoluteJoint(1).Rz.Pos = 0.0;
smiData.RevoluteJoint(1).ID = '';

smiData.RevoluteJoint(1).Rz.Pos = -179.44841488716256;  % deg
smiData.RevoluteJoint(1).ID = '[pendulum_assembly:2:-:cart_assembly:2]';

