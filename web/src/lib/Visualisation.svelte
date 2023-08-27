<script lang="ts">
    import vectorLogo from '../assets/Vector.png'
    import {onMount} from "svelte";
    type Point = {
        x: number;
        y: number;
        color: number;
    };

    const numberLocations = [{"x": 130, "y":14},{"x": 210, "y":14},{"x": 290, "y":14},{"x": 370, "y":14},
                             {"x": 492, "y":135},{"x": 492, "y":215},{"x": 492, "y":295},{"x": 492, "y":375},
                             {"x": 370, "y":495},{"x": 290, "y":495},{"x": 210, "y":495},{"x": 130, "y":495},
                             {"x": 9, "y":375},{"x": 9, "y":295},{"x": 9, "y":215},{"x": 9, "y":135}]

    const squareLocations = [{"x": 0, "y": 0, "color":0},{"x": 80, "y": 0, "color":0},{"x": 160, "y": 0, "color":0},{"x": 240, "y": 0, "color":0},{"x": 320, "y": 0, "color":0},{"x": 400, "y": 0, "color":0}, {"x": 480, "y": 0, "color":0},
                             {"x": 480, "y": 80, "color":0},{"x": 480, "y": 160, "color":0},{"x": 480, "y": 240, "color":0},{"x": 480, "y": 320, "color":0},{"x": 480, "y": 400, "color":0}, {"x": 480, "y": 480, "color":0},
                             {"x": 400, "y": 480, "color":0},{"x": 320, "y": 480, "color":0},{"x": 240, "y": 480, "color":0},{"x": 160, "y": 480, "color":0},{"x": 80, "y": 480, "color":0},{"x": 0, "y": 480, "color":0},
                             {"x": 0, "y": 400, "color":0},{"x": 0, "y": 320, "color":0},{"x": 0, "y": 240, "color":0},{"x": 0, "y": 160, "color":0},{"x": 0, "y": 80,"color":0}]
    const color = ["#8edff8","#ca3a30","#4ca053"]
    const textColor = ["black","white","white"]

    let topMeasurement = [233, 198, 442, 384]
    let botMeasurement = [395, 430, 500, 395]

    //let topMeasurement = [420, 205, 170, 318]
    //let botMeasurement = [500, 443, 432, 409]

    //let topMeasurement = [292, 264, 486, 500]
    //let botMeasurement = [361, 417, 486, 472]

    let myCanvas;


    export function DrawBoxes(ptns) {
        const sq = structuredClone(squareLocations);
        if (ptns[0] !== 24) {
            sq[ptns[0]].color = 1;
            sq[ptns[1]].color = 1;
            sq[ptns[2]].color = 2;
            sq[ptns[3]].color = 2;
        }

        var ctx = myCanvas.getContext("2d");

        ctx.lineWidth=3.0;
        ctx.strokeStyle = "red";
        ctx.beginPath();


        ctx.fillStyle = "#F0F0F0";
        ctx.fillRect(20, 20, 460, 460);

        for (let i = 0; i < 24; i++) {
            ctx.fillStyle = color[sq[i].color];
            ctx.fillRect(sq[i].x, sq[i].y, 20, 20);
            ctx.font = "12px Arial";
            ctx.fillStyle = textColor[sq[i].color];
            ctx.textAlign = "center";
            ctx.fillText(i, sq[i].x+10, sq[i].y+15);
        }
    }

    export function DrawCurves(msr)
    {
        var ctx = myCanvas.getContext("2d");
        ctx.lineWidth=3.0;
        ctx.strokeStyle = "red";
        ctx.beginPath();


        ctx.fillStyle = "#F0F0F0";
        ctx.fillRect(20, 20, 460, 460);

        for (let i = 0; i < 24; i++) {
            ctx.fillStyle = color[squareLocations[i].color];
            ctx.fillRect(squareLocations[i].x, squareLocations[i].y, 20, 20);
        }

        for (let i = 0; i < 16; i++) {
            ctx.font = "12px Arial";
            ctx.fillStyle = "black";
            ctx.textAlign = "center";
            ctx.fillText(i, numberLocations[i].x, numberLocations[i].y);
        }




        ctx.beginPath();
        ctx.strokeStyle = "red";
        for (let i = 0; i < 4; i++) {
            ctx.moveTo(100+80*i, 20);
            ctx.quadraticCurveTo(130+80*i, msr[0][i], 160+80*i, 20);
        }
        ctx.stroke();

        ctx.beginPath();
        ctx.strokeStyle = "green";
        for (let i = 0; i < 4; i++) {
            ctx.moveTo(480, 100+80*i);
            ctx.quadraticCurveTo(500-msr[1][i], 130+80*i, 480, 160+80*i);
        }
        ctx.stroke();

        ctx.beginPath();
        ctx.strokeStyle = "red";
        for (let i = 0; i < 4; i++) {
            ctx.moveTo(100+80*i, 480);
            ctx.quadraticCurveTo(130+80*i, 500-msr[2][i], 160+80*i, 480);
        }
        ctx.stroke();
        ctx.beginPath();
        ctx.strokeStyle = "green";
        for (let i = 0; i < 4; i++) {
            ctx.moveTo(20, 100+80*i);
            ctx.quadraticCurveTo(msr[3][i], 130+80*i, 20, 160+80*i);
        }
        ctx.stroke();
    }



</script>

<canvas class="aspect-square w-full bg-white"
        bind:this={myCanvas}
        width="500" height="500">
</canvas>
