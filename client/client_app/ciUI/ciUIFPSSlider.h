/********************************************************************************** 
 
 Copyright (C) 2012 Syed Reza Ali (www.syedrezaali.com)
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 of the Software, and to permit persons to whom the Software is furnished to do
 so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 
 **********************************************************************************/

#ifndef CIUI_FPS_SLIDER
#define CIUI_FPS_SLIDER

#include "cinder/app/App.h"
#include "ciUISlider.h"

class ciUIFPSSlider : public ciUISlider
{
public:        
    ciUIFPSSlider(float x, float y, float w, float h, float _min, float _max, float _value, string _name) : ciUISlider(x, y, w, h, _min, _max, _value, _name)
    {
        
    }
    
    ciUIFPSSlider(float w, float h, float _min, float _max, float _value, string _name) : ciUISlider(w, h, _min, _max, _value, _name)
    {

    }    

    ciUIFPSSlider(float x, float y, float w, float h, float _min, float _max, float *_value, string _name) : ciUISlider(x, y, w, h, _min, _max, _value, _name)
    {
    
    }
    
    ciUIFPSSlider(float w, float h, float _min, float _max, float *_value, string _name) : ciUISlider(w, h, _min, _max, _value, _name)
    {
        
    }   
    
    void update()
	{
        setValue(ci::app::App::get()->getAverageFps());
	}
	
}; 

#endif