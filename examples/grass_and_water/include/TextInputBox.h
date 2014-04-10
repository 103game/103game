#ifndef TEXTINPUTBOX_CLASS_DEF
#define TEXTINPUTBOX_CLASS_DEF
#include "Cursor.h"
#include <windows.h>

#include <cinder/app/AppNative.h>
#include <cinder/gl/gl.h>
#include <cinder/gl/Texture.h>
#include <cinder/Text.h>
#include <cinder/Utilities.h>
#include <cinder/Font.h>



class TextInputBox {
    int coursorPosition;

protected:
    gl::Texture renderedTextBox;

    void UpdateCoursorPosition() {
        auto symbols = text.measureGlyphs();
        auto font = text.getFont();

        auto isLast = symbols.size() == coursorPosition;

        if(isLast) {
            auto symbol = symbols[coursorPosition-1];
            auto symbolBox = font.getGlyphBoundingBox(font.getGlyphIndex(symbol.first));
            cursor.transform.position = transform.position + symbol.second + Vec2f( symbolBox.x2, 0);
        } else { 
            auto symbol = symbols[coursorPosition];
            cursor.transform.position = transform.position + symbol.second;
        }

        cursor.height = font.getSize();
    }

    void ChangeCoursorPosition(int value) {
        auto newPosition = coursorPosition + value;
        if(newPosition >= 0 && newPosition <= text.getText().size()) {
            coursorPosition = newPosition;
            UpdateCoursorPosition();
        }
    }

public:
    bool shalUpdateText;
    Transform transform;
    TextBox text;
    Cursor cursor;
	char symb;
	string aaa;

    TextInputBox(Vec2f size, std::string message = "", Font font = Font("Courier New", 15)) : coursorPosition(0), shalUpdateText(true){
        text.setSize(size);
        text.setFont(font);
        text.setText(message);
        text.setColor( Color( 1, 1, 1 ) );
        text.setBackgroundColor(ColorA(1, 0, 0.1f ));

        ChangeCoursorPosition(message.size());
    }

    void KeyDown( KeyEvent event ) {
        auto key =  event.getCode();
        switch(key) {
            case KeyEvent::KEY_RIGHT:
                ChangeCoursorPosition(1);
                break;
            case KeyEvent::KEY_LEFT:
                ChangeCoursorPosition(-1);
                break;
            default:    // добавте логику сохранения символа в массив текста и отоброжения только вмещающейся в размер текстового редактора, ограничив ввод до одной строки. для этого используйте например text.calculateLineBreaks (остовляя только первую строку)
               
				aaa = (char)key;
			   //ChangeCoursorPosition(1);
			   text.setText(aaa);

				break;
        }       
    }

    void Update(){
        if(shalUpdateText) {
            shalUpdateText = !shalUpdateText;
            renderedTextBox = gl::Texture(text.render());
        }
    }

    void Render(){
        gl::draw( renderedTextBox, transform.position );
        cursor.Render();
    }
};
#endif