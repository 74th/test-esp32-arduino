import { WebSocketConnector } from './connector';
import { useState, useEffect } from 'react';

function Key({ ws, text, code, width }: { ws: WebSocketConnector, text: string, code?: string, width: string }) {
    const [touched, setTouched] = useState(false);

    if (code === undefined) {
        code = text;
    }

    const handleClick = (e: React.MouseEvent<HTMLDivElement>) => {
        ws.SendKey(code);
    };

    const handleTouchStart = (e: React.TouchEvent<HTMLDivElement>) => {
        setTouched(true);
    }
    const handleTouchEnd = (e: React.TouchEvent<HTMLDivElement>) => {
        setTouched(false);
    }

    return (
        <div
            style={{ width: width, height: "100%", border: '1px solid white', backgroundColor: touched ? 'gray':'black', display: 'flex', alignItems: 'center', justifyContent: 'center' }}
            onClick={handleClick}
            onTouchStart={handleTouchStart}
            onTouchEnd={handleTouchEnd}
            tabIndex={0}
        >{ text }</div>
    );
}

function ModifierKey({ ws, text, keyName, width }: { ws: WebSocketConnector, text: string, keyName: "shift"| "ctrl"| "alt"| "win", width: string }) {
    const [pushed, setPushed] = useState(false);

    useEffect(() => {
        return () => {
            if(pushed) {
                ws.SetModifierKey(keyName, false);
            }
        };
    }, []);

    const handleClick = (e: React.MouseEvent<HTMLDivElement>) => {
        if (pushed) {
            ws.SetModifierKey(keyName, false);
        } else {
            ws.SetModifierKey(keyName, true);
        }
        setPushed(!pushed);
    };

    return (
        <div
            style={{ width: width, height: "100%", border: '1px solid white', backgroundColor: pushed ? 'gray' : 'black', display: 'flex', alignItems: 'center', justifyContent: 'center' }}
            onClick={handleClick}
            tabIndex={0}
        >{ text }</div>
    );
}

export function Keyboard({ ws }: { ws: WebSocketConnector }) {
    return (
        <div
            style={{ width: "99%", height: "99%"}}
        >
            <div style={{ width: "99%", height: "19.8%", display: "flex", flexDirection: "row" }}>
                <Key text="1" ws={ws} width="9.9%"></Key>
                <Key text="2" ws={ws} width="9.9%"></Key>
                <Key text="3" ws={ws} width="9.9%"></Key>
                <Key text="4" ws={ws} width="9.9%"></Key>
                <Key text="5" ws={ws} width="9.9%"></Key>
                <Key text="6" ws={ws} width="9.9%"></Key>
                <Key text="7" ws={ws} width="9.9%"></Key>
                <Key text="8" ws={ws} width="9.9%"></Key>
                <Key text="9" ws={ws} width="9.9%"></Key>
                <Key text="0" ws={ws} width="9.9%"></Key>
            </div>
            <div style={{ width: "99%", height: "19.8%", display: "flex", flexDirection: "row" }}>
                <Key text="Q" ws={ws} width="9.9%"></Key>
                <Key text="W" ws={ws} width="9.9%"></Key>
                <Key text="E" ws={ws} width="9.9%"></Key>
                <Key text="R" ws={ws} width="9.9%"></Key>
                <Key text="T" ws={ws} width="9.9%"></Key>
                <Key text="Y" ws={ws} width="9.9%"></Key>
                <Key text="U" ws={ws} width="9.9%"></Key>
                <Key text="I" ws={ws} width="9.9%"></Key>
                <Key text="O" ws={ws} width="9.9%"></Key>
                <Key text="P" ws={ws} width="9.9%"></Key>
            </div>
            <div style={{ width: "99%", height: "19.8%", display: "flex", flexDirection: "row" }}>
                <Key text="A" ws={ws} width="9.9%"></Key>
                <Key text="S" ws={ws} width="9.9%"></Key>
                <Key text="D" ws={ws} width="9.9%"></Key>
                <Key text="F" ws={ws} width="9.9%"></Key>
                <Key text="G" ws={ws} width="9.9%"></Key>
                <Key text="H" ws={ws} width="9.9%"></Key>
                <Key text="J" ws={ws} width="9.9%"></Key>
                <Key text="K" ws={ws} width="9.9%"></Key>
                <Key text="L" ws={ws} width="9.9%"></Key>
                <Key text="." ws={ws} width="9.9%"></Key>
            </div>
            <div style={{ width: "99%", height: "19.8%", display: "flex", flexDirection: "row" }}>
                <ModifierKey text="SFT" keyName="shift" ws={ws} width="14.85%"></ModifierKey>
                <Key text="Z" ws={ws} width="9.9%"></Key>
                <Key text="X" ws={ws} width="9.9%"></Key>
                <Key text="C" ws={ws} width="9.9%"></Key>
                <Key text="V" ws={ws} width="9.9%"></Key>
                <Key text="B" ws={ws} width="9.9%"></Key>
                <Key text="N" ws={ws} width="9.9%"></Key>
                <Key text="M" ws={ws} width="9.9%"></Key>
                <Key text="ENT" code="RETURN" ws={ws} width="14.85%"></Key>
            </div>
            <div style={{ width: "99%", height: "19.8%", display: "flex", flexDirection: "row" }}>
                <Key text="=%/" code="SPACE" ws={ws} width="19.8%"></Key>
                <ModifierKey text="CTL" keyName="ctrl" ws={ws} width="9.9%"></ModifierKey>
                <Key text="SPACE" code="SPACE" ws={ws} width="24.75%"></Key>
                <ModifierKey text="ALT" keyName="alt" ws={ws} width="9.9%"></ModifierKey>
                <ModifierKey text="WIN" keyName="win" ws={ws} width="9.9%"></ModifierKey>
                <Key text="TAB" ws={ws} width="9.9%"></Key>
                <Key text="BS" code="BACK_SPACE" ws={ws} width="14.85%"></Key>
            </div>

        </div>
    )
}