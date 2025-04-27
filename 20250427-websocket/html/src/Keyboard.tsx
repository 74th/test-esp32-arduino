import { WebSocketConnector } from './connector';

function Key({ ws, text, code }: { ws: WebSocketConnector, text: string, code: number }) {
    const handleKeyDown = (e: React.KeyboardEvent) => {
        if (e.key === 'Enter') {
            ws.sendKey('Enter');
        } else if (e.key === 'Escape') {
            ws.sendKey('Escape');
        } else {
            ws.sendKey(e.key);
        }
    };

    return (
        <div
            style={{ width: "100%", height: "100%"}}
            onKeyDown={handleKeyDown}
            tabIndex={0}
        ></div>
    );
}

function Keyboard({ ws }: { ws: WebSocketConnector }) {
    return (
        <div
            style={{ width: "100%", height: "100%"}}
        ></div>
    )
}