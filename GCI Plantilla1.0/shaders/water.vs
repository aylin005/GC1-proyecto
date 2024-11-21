#version 400

layout(location = 0) in vec3 inputPosition; // Posición del vértice
layout(location = 1) in vec2 inputTexCoord; // Coordenadas de textura (opcional)

uniform mat4 worldMatrix;       // Matriz del modelo
uniform mat4 viewMatrix;        // Matriz de la cámara
uniform mat4 projectionMatrix;  // Matriz de proyección
uniform float uTime;       // Tiempo para animar las ondas

out vec2 texCoord; // Coordenadas de textura pasadas al fragment shader

void main() {
    // Parámetros para las ondas
    float waveAmplitude = 0.5; // Altura de las ondas
    float waveFrequency = 2.0; // Frecuencia de las ondas
    float waveSpeed = 1.5;     // Velocidad de las ondas

    // Calcula el desplazamiento vertical usando una función seno
    float wave = waveAmplitude * sin(waveFrequency * (inputPosition.x + inputPosition.z) - uTime * waveSpeed);

    // Aplica el desplazamiento al vértice
    vec3 newPosition = inputPosition;
    newPosition.y += wave;

    // Calcula la posición final del vértice
    gl_Position = projectionMatrix * viewMatrix * worldMatrix * vec4(newPosition, 1.0);

    // Pasa las coordenadas de textura al fragment shader
    texCoord = inputTexCoord;
}