function byteToHexString(byteArray: Uint8Array): string {
    return Array.from(byteArray)
        .map((byte) => byte.toString(16).padStart(2, '0'))
        .join(' ');
}

export class WebSocketConnector {
    private ws: WebSocket | null = null;
    private url: string;
    public onopen: (event: Event) => void = () => { };
    public onclose: (event: Event) => void = () => { };
    private left_click: boolean = false;
    private right_click: boolean = false;
    private middle_click: boolean = false;

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

    SendText(message: string) {
        console.log("start send");
        if (this.ws && this.ws.readyState === WebSocket.OPEN) {
            this.ws.send(message);
        } else {
            console.warn('WebSocket is not open');
        }
    }

    SendMouseMove(dx: number, dy: number, wy: number) {
        console.log("start send");
        if (!this.ws || this.ws.readyState !== WebSocket.OPEN) {
            this.connect();
            return;
        }

        this.sendMouseCode(dx, dy, wy);
    }

    sendMouseCode(dx: number = 0, dy: number = 0, wy: number = 0) {
        console.log("start send");
        if (!this.ws || this.ws.readyState !== WebSocket.OPEN) {
            this.connect();
            return;
        }

        const bytes = new Uint8Array([0x57, 0xAB, 0x00, 0x05, 0x05, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00]);
        if (this.left_click) {
            bytes[6] |= 0x01;
        }
        if (this.right_click) {
            bytes[6] |= 0x02;
        }
        if (this.middle_click) {
            bytes[6] |= 0x04;
        }
        if (dx > 0) {
            bytes[7] = 1;
        }
        if (dx < 0) {
            bytes[7] = -1;
        }
        if (dy > 0) {
            bytes[8] = 1;
        }
        if (dy < 0) {
            bytes[8] = -1;
        }
        if (wy < 0) {
            bytes[9] = -1;
        }
        if (wy > 0) {
            bytes[9] = 1;
        }

        // checksum
        for (let i = 0; i < 10; i++) {
            bytes[10] += bytes[i];
        }
        console.log("send mouse move", byteToHexString(bytes));

        this.ws.send(bytes);
    }

    SetMouseClick(button: "left" | "right" | "middle") {
        if (!this.ws || this.ws.readyState !== WebSocket.OPEN) {
            this.connect();
            return;
        }

        if (button === "left") {
            this.left_click = true;
        } else if (button === "right") {
            this.right_click = true;
        } else if (button === "middle") {
            this.middle_click = true;
        }


        this.sendMouseCode();
    }

    setReleaseMoudeClick() {
    }

    close() {
        console.log("@@2");
        if (this.ws) {
            this.ws.close();
        }
    }
}