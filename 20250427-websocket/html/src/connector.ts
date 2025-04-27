export class WebSocketConnector {
    private ws: WebSocket | null = null;
    private url: string;
    public onopen: (event: Event) => void = () => { };
    public onclose: (event: Event) => void = () => { };

    constructor(url: string) {
        this.url = url;
    }

    connect() {
        console.log("start connect");
        this.ws = new WebSocket(this.url);
        this.ws.onopen = () => {
            console.log('WebSocket connected');
            this.onopen(new Event('open'));
        };
        this.ws.onclose = () => {
            console.log('WebSocket disconnected');
            this.onclose(new Event('close'));
        };
        this.ws.onerror = (err) => {
            console.error('WebSocket error', err);
        };
        this.ws.onmessage = (event) => {
            console.log('WebSocket message:', event.data);
        };
    }

    send(message: string) {
        console.log("start send");
        if (this.ws && this.ws.readyState === WebSocket.OPEN) {
            this.ws.send(message);
        } else {
            console.warn('WebSocket is not open');
        }
    }

    sendMouseMove(dx: number, dy: number) {
        console.log("start send");
        if (!this.ws || this.ws.readyState !== WebSocket.OPEN) {
            this.connect();
            return;
        }

        const bytes = new Uint8Array([77, 0, 0]);
        if (dx > 0) {
            bytes[1] = 1;
        }
        if (dx < 0) {
            bytes[1] = -1;
        }
        if (dy > 0) {
            bytes[2] = 1;
        }
        if (dy < 0) {
            bytes[2] = -1;
        }

        this.ws.send(bytes);
    }

    close() {
        console.log("@@2");
        if (this.ws) {
            this.ws.close();
        }
    }
}