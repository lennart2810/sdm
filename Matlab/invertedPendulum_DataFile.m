% Simscape(TM) Multibody(TM) version: 7.4

% This is a model data file derived from a Simscape Multibody Import XML file using the smimport function.
% The data in this file sets the block parameter values in an imported Simscape Multibody model.
% For more information on this file, see the smimport function help page in the Simscape Multibody documentation.
% You can modify numerical values, but avoid any other changes to this file.
% Do not add code to this file. Do not edit the physical units shown in comments.

%%%VariableName:smiData


%============= RigidTransform =============%

%Initialize the RigidTransform structure array by filling in null values.
smiData.RigidTransform(8).translation = [0.0 0.0 0.0];
smiData.RigidTransform(8).angle = 0.0;
smiData.RigidTransform(8).axis = [0.0 0.0 0.0];
smiData.RigidTransform(8).ID = '';

%Translation Method - Cartesian
%Rotation Method - Arbitrary Axis
smiData.RigidTransform(1).translation = [2.2204460492503131e-15 0 -1.1102230246251565e-15];  % mm
smiData.RigidTransform(1).angle = 0;  % rad
smiData.RigidTransform(1).axis = [0 0 0];
smiData.RigidTransform(1).ID = 'B[pendulum:1:-:cart:1]';

%Translation Method - Cartesian
%Rotation Method - Arbitrary Axis
smiData.RigidTransform(2).translation = [-7.1054273576010019e-15 14.99999999999997 9.9999999999999822];  % mm
smiData.RigidTransform(2).angle = 0;  % rad
smiData.RigidTransform(2).axis = [0 0 0];
smiData.RigidTransform(2).ID = 'F[pendulum:1:-:cart:1]';

%Translation Method - Cartesian
%Rotation Method - Arbitrary Axis
smiData.RigidTransform(3).translation = [0 0 0];  % mm
smiData.RigidTransform(3).angle = 2.0943951023931953;  % rad
smiData.RigidTransform(3).axis = [0.57735026918962584 0.57735026918962584 0.57735026918962584];
smiData.RigidTransform(3).ID = 'B[:-:base:1]';

%Translation Method - Cartesian
%Rotation Method - Arbitrary Axis
smiData.RigidTransform(4).translation = [0 0 0];  % mm
smiData.RigidTransform(4).angle = 2.0943951023931953;  % rad
smiData.RigidTransform(4).axis = [0.57735026918962584 0.57735026918962584 -0.57735026918962584];
smiData.RigidTransform(4).ID = 'F[:-:base:1]';

%Translation Method - Cartesian
%Rotation Method - Arbitrary Axis
smiData.RigidTransform(5).translation = [0 0 0];  % mm
smiData.RigidTransform(5).angle = 0;  % rad
smiData.RigidTransform(5).axis = [0 0 0];
smiData.RigidTransform(5).ID = 'B[cart:1:-:]';

%Translation Method - Cartesian
%Rotation Method - Arbitrary Axis
smiData.RigidTransform(6).translation = [-34.999999999999972 -3.6386842403929371 -2.1431318985078679e-15];  % mm
smiData.RigidTransform(6).angle = 0;  % rad
smiData.RigidTransform(6).axis = [0 0 0];
smiData.RigidTransform(6).ID = 'F[cart:1:-:]';

%Translation Method - Cartesian
%Rotation Method - Arbitrary Axis
smiData.RigidTransform(7).translation = [15.000000000000027 0 0];  % mm
smiData.RigidTransform(7).angle = 2.0943951023931953;  % rad
smiData.RigidTransform(7).axis = [0.57735026918962584 0.57735026918962584 0.57735026918962584];
smiData.RigidTransform(7).ID = 'B[cart:1:-:base:1]';

%Translation Method - Cartesian
%Rotation Method - Arbitrary Axis
smiData.RigidTransform(8).translation = [-3.5527136788005009e-15 19.999999999999943 3.6386842403929385];  % mm
smiData.RigidTransform(8).angle = 2.0943951023931953;  % rad
smiData.RigidTransform(8).axis = [0.57735026918962584 -0.57735026918962584 0.57735026918962584];
smiData.RigidTransform(8).ID = 'F[cart:1:-:base:1]';


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

%Inertia Type - Custom
%Visual Properties - Simple
smiData.Solid(1).mass = 0.07558230046613619;  % kg
smiData.Solid(1).CoM = [-3.4121970144518806e-11 14.999999999999998 5.5873161298538996];  % mm
smiData.Solid(1).MoI = [6.3180774355951943 6.3180774435576232 10.659153751845492];  % kg*mm^2
smiData.Solid(1).PoI = [0 2.1693665836154424e-11 0];  % kg*mm^2
smiData.Solid(1).color = [0.70196078431372544 0 0];
smiData.Solid(1).opacity = 1;
smiData.Solid(1).ID = 'cart.ipt_{54C29851-48B7-4460-5733-7C9EF68764EF}';

%Inertia Type - Custom
%Visual Properties - Simple
smiData.Solid(2).mass = 0.089267699533863834;  % kg
smiData.Solid(2).CoM = [2.8889257474024575e-11 -42.210116533479216 3.9999999999999996];  % mm
smiData.Solid(2).MoI = [70.617548427422932 2.2115175176099853 71.876877150005029];  % kg*mm^2
smiData.Solid(2).PoI = [0 0 -1.0885472216212461e-10];  % kg*mm^2
smiData.Solid(2).color = [0.094117647058823528 0.14509803921568629 0.97254901960784312];
smiData.Solid(2).opacity = 1;
smiData.Solid(2).ID = 'pendulum.ipt_{23D014E0-41FC-C027-13E1-07A39EBF00BE}';

%Inertia Type - Custom
%Visual Properties - Simple
smiData.Solid(3).mass = 0.47099999999999975;  % kg
smiData.Solid(3).CoM = [5.0000000000000018 10 1.8947806286936013e-14];  % mm
smiData.Solid(3).MoI = [3548.1999999999971 3536.424999999997 19.624999999999989];  % kg*mm^2
smiData.Solid(3).PoI = [0 0 0];  % kg*mm^2
smiData.Solid(3).color = [0.8784313725490196 0.87450980392156863 0.85882352941176465];
smiData.Solid(3).opacity = 1;
smiData.Solid(3).ID = 'base.ipt_{59FCD0DD-4C58-99B2-AE00-CFB0293BBF18}';


%============= Joint =============%
%X Revolute Primitive (Rx) %Y Revolute Primitive (Ry) %Z Revolute Primitive (Rz)
%X Prismatic Primitive (Px) %Y Prismatic Primitive (Py) %Z Prismatic Primitive (Pz) %Spherical Primitive (S)
%Constant Velocity Primitive (CV) %Lead Screw Primitive (LS)
%Position Target (Pos)

%Initialize the PlanarJoint structure array by filling in null values.
smiData.PlanarJoint(2).Rz.Pos = 0.0;
smiData.PlanarJoint(2).Px.Pos = 0.0;
smiData.PlanarJoint(2).Py.Pos = 0.0;
smiData.PlanarJoint(2).ID = '';

smiData.PlanarJoint(1).Rz.Pos = 0;  % deg
smiData.PlanarJoint(1).Px.Pos = 0;  % mm
smiData.PlanarJoint(1).Py.Pos = 0;  % mm
smiData.PlanarJoint(1).ID = '[cart:1:-:]';

%This joint has been chosen as a cut joint. Simscape Multibody treats cut joints as algebraic constraints to solve closed kinematic loops. The imported model does not use the state target data for this joint.
smiData.PlanarJoint(2).Rz.Pos = 179.99999999999997;  % deg
smiData.PlanarJoint(2).Px.Pos = 0;  % mm
smiData.PlanarJoint(2).Py.Pos = 0;  % mm
smiData.PlanarJoint(2).ID = '[cart:1:-:base:1]';


%Initialize the RevoluteJoint structure array by filling in null values.
smiData.RevoluteJoint(1).Rz.Pos = 0.0;
smiData.RevoluteJoint(1).ID = '';

smiData.RevoluteJoint(1).Rz.Pos = -90.030150161315035;  % deg
smiData.RevoluteJoint(1).ID = '[pendulum:1:-:cart:1]';

