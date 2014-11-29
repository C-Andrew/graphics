require('readobj')

regice = gr.mesh('regice', readobj('regice.obj'))
factor = 2.0/(2.76+3.637)

regice:set_material(Aqua)

regice:translate(0.0, -1.0, 0.0)
regice:scale(factor, factor, factor)
regice:translate(0.0, 3.637, 0.0)



regice_instance = gr.node('regice')



-- Vertical
eye1 = gr.sphere('eye1')
eye1:set_material(Yellow)
eye1:translate(0, 4, 1.2)
eye1:scale(0.12,0.12,0.12)

eye2 = gr.sphere('eye2')
eye2:set_material(Yellow)
eye2:translate(0, 4.5, 1.2)
eye2:scale(0.12,0.12,0.12)

eye3 = gr.sphere('eye3')
eye3:set_material(Yellow)
eye3:translate(0, 5, 1.1)
eye3:scale(0.12,0.12,0.12)
  
-- Horizontal Left
eye4 = gr.sphere('eye4')
eye4:set_material(Yellow)
eye4:translate(-0.38, 4.5, 1.0)
eye4:scale(0.12,0.12,0.12)

eye5 = gr.sphere('eye5')
eye5:set_material(Yellow)
eye5:translate(-0.7, 4.5, 0.85)
eye5:scale(0.12,0.12,0.12)

-- Horizontal Right
eye6 = gr.sphere('eye6')
eye6:set_material(Yellow)
eye6:translate(0.4, 4.5, 1.0)
eye6:scale(0.12,0.12,0.12)

eye7 = gr.sphere('eye7')
eye7:set_material(Yellow)
eye7:translate(0.7, 4.5, 0.85)
eye7:scale(0.12,0.12,0.12)