#pragma once

#include <Windows.h>

class TouchInput
{
public :

    int prevX;
    int prevY;

    int jitterRadius;

    bool isDrag;

    TouchInput()
    {
        prevX = 0;
        prevY = 0;

        jitterRadius = 50;

        isDrag = false;
    }

    void Input( POINTER_FLAGS pointerFlags, int x, int y )
    {
        POINTER_TOUCH_INFO contact = { 0 };
        contact.pointerInfo.pointerType = PT_TOUCH;
        contact.pointerInfo.pointerFlags = pointerFlags;
        contact.pointerInfo.ptPixelLocation.x = x;
        contact.pointerInfo.ptPixelLocation.y = y;
        contact.pointerInfo.pointerId = 0;

        BOOL ret = ::InjectTouchInput( 1, &contact );
        if ( !ret ) {
            //throw std::runtime_error( __FUNCTION__ );
        }

        prevX = x;
        prevY = y;
    }

    void Down( int x, int y ) 
    {
        Input( POINTER_FLAG_INRANGE | POINTER_FLAG_INCONTACT | POINTER_FLAG_DOWN, x, y );

        isDrag = false;
    }

    void Drag( int x, int y )
    {
        if ( !isDrag ) {
            // 置いてから一定範囲移動しないとドラッグとみなさないようにする
            // 動かすとタッチと認識しないため
            RECT rc = { prevX - jitterRadius, prevY - jitterRadius,
                        prevX + jitterRadius, prevY + jitterRadius };
            POINT pt = { x, y };
            if ( ::PtInRect( &rc, pt ) ) {
                Input( POINTER_FLAG_INRANGE | POINTER_FLAG_INCONTACT | POINTER_FLAG_UPDATE, prevX, prevY );
                return;
            }

            isDrag = true;
        }

        Input( POINTER_FLAG_INRANGE | POINTER_FLAG_INCONTACT | POINTER_FLAG_UPDATE, x, y );
    }

    void Up()
    {
        // UpのX,Y座標は直前の座標が入っていないと、パラメーターエラーになる
        Input( POINTER_FLAG_INRANGE | POINTER_FLAG_UP, prevX, prevY );
    }
    
    void Hover( int x, int y )
    {
        Input( POINTER_FLAG_INRANGE | POINTER_FLAG_UPDATE, x, y );
    }

    void HoverUp()
    {
        Input( POINTER_FLAG_UPDATE, prevX, prevY );
    }

    static void Initialize( UINT32 maxCount = 10, DWORD dwMode = TOUCH_FEEDBACK_INDIRECT )
    {
        auto ret = ::InitializeTouchInjection( maxCount, dwMode );
        if ( !ret ) {
            throw std::runtime_error( "InitializeTouchInjection" );
        }
    }
};

