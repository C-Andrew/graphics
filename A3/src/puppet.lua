--
-- CS488 -- Introduction to Computer Graphics
-- 
-- a3mark.lua
--
-- A very simple scene creating a trivial puppet.  The TAs will be
-- using this scene as part of marking your assignment.  You'll
-- probably want to make sure you get reasonable results with it!

rootnode = gr.node('root')

red = gr.material({1.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 10)
darkRed = gr.material({151/255, 10/255, 19/255}, {0.1, 0.1, 0.1}, 10)
blue = gr.material({0.0, 0.0, 1.0}, {0.1, 0.1, 0.1}, 10)
green = gr.material({0.0, 1.0, 0.0}, {0.1, 0.1, 0.1}, 10)
white = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 10)
black = gr.material({0, 0, 0}, {0.1, 0.1, 0.1}, 10)
lightBlue = gr.material({0.262745098, 0.592156863, 0.968627451}, {0.1, 0.1, 0.1}, 10)
purple = gr.material({145/255, 0.0, 1.0}, {0.1, 0.1, 0.1}, 10)
gold = gr.material({230/255, 223/255, 14/255}, {0.1, 0.1, 0.1}, 10)

-- gr.joint('name', {set_joint_x}, {set_joint_y})
-- set_joint_x/y (double min, double init, double max)
torsoJoint = gr.joint('torsoJoint', {0.0, 0.0, 0.0}, {0.0,0.0,0.0})
rootnode:add_child(torsoJoint)

hipJoint = gr.joint('hipJoint', {0.0, 0.0, 0.0}, {0.0,0.0,0.0})
hipJoint:translate(0.0, -1.0, 0.0)
torsoJoint:add_child(hipJoint)

-- Torso
torso = gr.sphere('torso')
torsoJoint:add_child(torso)
torso:translate(0.0, 2.0, 0.0)
torso:scale(1.0,3.0,1.0)
torso:set_material(blue)

shoulderJoint = gr.joint('shoulderJoint', {0.0, 0.0, 0.0}, {0.0,0.0,0.0})
torsoJoint:add_child(shoulderJoint)
shoulderJoint:translate(0.0, 5.0, 0.0)

-- Neck
neckJoint = gr.joint('shoulderJoint', {-5.0, 0.0, 5.0}, {0.0,0.0,0.0})
shoulderJoint:add_child(neckJoint)
neckJoint:translate(0.0, 1.0, 0.0)

neck = gr.sphere('neck')
neckJoint:add_child(neck)
neck:scale(0.7, 1.0, 0.7)
neck:set_material(blue)

-- Head
headJoint = gr.joint('headJoint', {-5.0, 0.0, 5.0}, {-10.0,0.0,10.0})
neckJoint:add_child(headJoint)
headJoint:translate(0.0, 1.4, 0.0)

head = gr.sphere('head')
headJoint:add_child(head)
head:scale(1.3, 1.1, 1.3)
head:set_material(blue)

noseJoint = gr.joint('noseJoint', {-3.0, 0.0, 3.0}, {0.0,0.0,0.0})
headJoint:add_child(noseJoint)
noseJoint:translate(0.0, 0.6, 1.0)

nose = gr.sphere('nose')
noseJoint:add_child(nose)
nose:scale(.1, 1.0, 0.4)
nose:set_material(red)

-- -- Shoulders
shoulder = gr.sphere('shoulder')
shoulderJoint:add_child(shoulder)
-- shoulder:translate(0.0, 5.0, 0.0)
shoulder:scale(4.0, 1.0, 2.0)
shoulder:set_material(black)

torsoLine = gr.sphere('torsoLine')
shoulderJoint:add_child(torsoLine)
-- torsoLine:translate(0.0, 5.0, 0.0)
torsoLine:scale(4.6, 0.3, 2.4)
torsoLine:set_material(gold)

-- Left Shoulder+Arm
leftShoulderJoint = gr.joint('leftShoulderJoint', {-30.0, 0.0, 30.0}, {0.0,0.0,0.0})
shoulderJoint:add_child(leftShoulderJoint)
leftShoulderJoint:translate(-3.5, 0.0, 0.0)

leftUpperArm = gr.sphere('leftUpperArm')
leftShoulderJoint:add_child(leftUpperArm)
leftUpperArm:scale(0.8, 2.4, 1.0)
leftUpperArm:translate(0.0, -1.0, 0.0)
leftUpperArm:set_material(red)

leftElbowJoint = gr.joint('leftElbowJoint', {-60.0, 0.0, 0.0}, {0.0,0.0,0.0})
leftShoulderJoint:add_child(leftElbowJoint)
leftElbowJoint:translate(0.0, -3.8, 0.0)

leftForeArm = gr.sphere('leftForeArm')
leftElbowJoint:add_child(leftForeArm)
leftForeArm:scale(0.6, 2.0, 1.0)
leftForeArm:translate(0.0, -1.0, 0.0);
leftForeArm:set_material(gold)

leftWrist = gr.joint('leftWrist', {-10.0, 0.0, 10.0}, {0.0,0.0,0.0})
leftElbowJoint:add_child(leftWrist)
leftWrist:translate(0.0, -3.9, 0.0)

leftHand = gr.sphere('leftHand')
leftWrist:add_child(leftHand)
leftHand:scale(0.6, 0.9, 0.6)
leftHand:set_material(darkRed)

-- Right Shoulder+Arm
rightShoulderJoint = gr.joint('rightShoulderJoint', {-30.0, 0.0, 30.0}, {0.0,0.0,0.0})
shoulderJoint:add_child(rightShoulderJoint)
rightShoulderJoint:translate(3.5, 0.0, 0.0)

rightUpperArm = gr.sphere('rightUpperArm')
rightShoulderJoint:add_child(rightUpperArm)
rightUpperArm:scale(0.8, 2.4, 1.0)
rightUpperArm:translate(0.0, -1.0, 0.0)
rightUpperArm:set_material(red)

rightElbowJoint = gr.joint('rightElbowJoint', {-60.0, 0.0, 0.0}, {0.0,0.0,0.0})
rightShoulderJoint:add_child(rightElbowJoint)
rightElbowJoint:translate(0.0, -3.8, 0.0)

rightForeArm = gr.sphere('rightForeArm')
rightElbowJoint:add_child(rightForeArm)
rightForeArm:scale(0.6, 2.0, 1.0)
rightForeArm:translate(0.0, -1.0, 0.0);
rightForeArm:set_material(gold)

rightWrist = gr.joint('rightWrist', {-5.0, 0.0, 5.0}, {0.0,0.0,0.0})
rightElbowJoint:add_child(rightWrist)
rightWrist:translate(0.0, -4.0, 0.0)

rightHand = gr.sphere('rightHand')
rightWrist:add_child(rightHand)
rightHand:scale(0.6, 0.9, 0.6)
rightHand:set_material(darkRed)

-- shoulder = gr.sphere('shoulder')
-- shoulderJoint:add_child(shoulder)
-- shoulder:translate(0.0, 1.0, 0.0)
-- shoulder:scale(10.0,2.0, 0.0)
-- shoulder:set_material(white)

-- Left Upper Arm
-- leftElbowJoint = gr.joint('leftElbowJoint', {0.0, 0.0, 0.0}, {0.0,0.0,0.0})
-- leftElbowJoint:translate(2.0, 2.0, 0.0)
-- leftShoulderJoint:add_child(leftElbowJoint)

-- leftUpperArm = gr.sphere('leftUpperArm')
-- leftShoulderJoint:add_child(leftUpperArm)
-- leftUpperArm:translate(0.0, 0.0, 0.0)
-- leftUpperArm:scale(1.0, 1.0, 1.0)
-- leftUpperArm:set_material(red)

-- Left Lower Arm
-- leftLowerArm = gr.sphere('leftLowerArm')
-- leftElbowJoint:add_child(leftLowerArm)
-- leftLowerArm:translate(-2.5, 0, 0.0)
-- leftLowerArm:scale(0.3, 1.5, 0.0)
-- leftLowerArm:set_material(red)

-- Right Upper Arm
-- Right Lower Arm


-- Hips
hips = gr.sphere('hips')
hipJoint:add_child(hips)
hips:scale(1.3, 1.0, 1.0)
hips:set_material(gold)

-- Left Leg
leftLegJoint = gr.joint('leftLegJoint', {-30.0, 0.0, 30.0}, {0.0,0.0,0.0})
hipJoint:add_child(leftLegJoint)
leftLegJoint:translate(-1.5, -0.5, 0.0)

leftThigh = gr.sphere('leftThigh')
leftLegJoint:add_child(leftThigh)
leftThigh:translate(0.0, -1.5, 0.0);
leftThigh:scale(0.8, 2.0, 1.0)
leftThigh:set_material(red)

leftKneeJoint = gr.joint('leftKneeJoint', {0.0, 0.0, 20.0}, {0.0,0.0,0.0})
leftLegJoint:add_child(leftKneeJoint)
leftKneeJoint:translate(0, -3.8, 0.0)

leftCalf = gr.sphere('leftCalf')
leftKneeJoint:add_child(leftCalf)
leftCalf:translate(0.0, -1.5, 0.0);
leftCalf:scale(0.8, 2.0, 1.0)
leftCalf:set_material(darkRed)

leftAnkle = gr.joint('leftAnkle', {-10.0, 0.0, 10.0}, {0.0,0.0,0.0})
leftKneeJoint:add_child(leftAnkle)
leftAnkle:translate(0, -3.5, 0.0)

leftFoot = gr.sphere('leftFoot')
leftAnkle:add_child(leftFoot)
leftFoot:translate(0.0, 0.0, 0.5)
leftFoot:scale(1.0, 0.5, 1.3)
leftFoot:scale(1.0, 0.5, 1.3)
leftFoot:set_material(black)

-- Right Leg
rightLegJoint = gr.joint('rightLegJoint', {-30.0, 0.0, 30.0}, {0.0,0.0,0.0})
hipJoint:add_child(rightLegJoint)
rightLegJoint:translate(1.5, -0.5, 0.0)

rightThigh = gr.sphere('rightThigh')
rightLegJoint:add_child(rightThigh)
rightThigh:translate(0.0, -1.5, 0.0);
rightThigh:scale(0.8, 2.0, 1.0)
rightThigh:set_material(red)

rightKneeJoint = gr.joint('rightKneeJoint', {0.0, 0.0, 20.0}, {0.0,0.0,0.0})
rightLegJoint:add_child(rightKneeJoint)
rightKneeJoint:translate(0, -3.8, 0.0)

righttCalf = gr.sphere('righttCalf')
rightKneeJoint:add_child(righttCalf)
righttCalf:translate(0.0, -1.5, 0.0);
righttCalf:scale(0.8, 2.0, 1.0)
righttCalf:set_material(darkRed)

rightAnkle = gr.joint('rightAnkle', {-10.0, 0.0, 10.0}, {0.0,0.0,0.0})
rightKneeJoint:add_child(rightAnkle)
rightAnkle:translate(0, -3.5, 0.0)

rightFoot = gr.sphere('rightFoot')
rightAnkle:add_child(rightFoot)
rightFoot:translate(0.0, 0.0, 0.5)
rightFoot:scale(1.0, 0.5, 1.3)
rightFoot:scale(1.0, 0.5, 1.3)
rightFoot:set_material(black)


-- leftShoulderJoint = gr.joint('leftShoulderJoint', {0.0, 0.0, 0.0}, {0.0,0.0,0.0})
-- leftShoulderJoint:translate(2.0, 2.0, 0.0)
-- shoulderJoint:add_child(leftShoulderJoint)

-- rightShoulderJoint = gr.joint('rightShoulderJoint', {0.0, 0.0, 0.0}, {0.0,0.0,0.0})
-- rightShoulderJoint:translate(2.0, 2.0, 0.0)
-- shoulderJoint:add_child(rightShoulderJoint)

-- shoulder = gr.sphere('shoulder')
-- shoulderJoint:add_child(shoulder)
-- shoulder:translate(0.0, 0.0, 0.0)
-- shoulder:scale(1.0, 1.0, 1.0)
-- shoulder:set_material(white)




-- -- Left Thigh
-- leftKneeJoint = gr.joint('leftKneeJoint', {0.0, 0.0, 0.0}, {0.0,0.0,0.0})
-- leftElbowJoint:translate(2.0, 2.0, 0.0)
-- leftLegJoint:add_child(leftKneeJoint)

-- leftUpperArm = gr.sphere('leftUpperArm')
-- leftShoulderJoint:add_child(leftUpperArm)
-- leftUpperArm:translate(-2.5, 3.8, 0.0)
-- leftUpperArm:scale(0.4, 2.3, 1.0)
-- leftUpperArm:set_material(red)

-- Left calf	


-- s2 = gr.sphere('s2')
-- rootnode:add_child(s2)
-- s2:translate(2.0, -2.0, 0.0)
-- s2:rotate('z', -90.0)
-- s2:scale(0.1, 2.0, 0.1)
-- s2:set_material(blue)

-- s3 = gr.sphere('s3')
-- rootnode:add_child(s3)
-- s3:scale(0.1, 0.1, 2.0)
-- s3:translate(0.0, -20.0, 1.0)
-- s3:set_material(green)


-- rootnode:translate(-0.75, 0.25, -10.0)
-- rootnode:rotate('y', -20.0)
-- rootnode:scale(2.0, 4.0, 0.1)

return rootnode
