#version 400

in vec3 inputPosition; //posicion del agua
in vec3 inputNormal; //normales 
in vec2 inputTexCoord; //coords de textur 
in vec3 inputTangent; //para mapeado
in vec3 inputBinormal;

out vec2 texCoord; //se envian al ps -> coords textura
out vec3 normal;
out vec3 tangente;
out vec3 binormal;
out vec3 position;
out vec3 viewDirec; //direccion de la camara

//parametros
uniform sampler2D dTex; //heightmap
uniform mat4 worldMatrix; 
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 camDir; //direccion de la camara
uniform vec3 waves; //movimiento de las olas
uniform float speed; //velocidad

void main(void)
{
    texCoord = inputTexCoord;

    vec4 height = texture(dTex, (texCoord * 3.0) + vec2(speed / 1000.0)); 

    vec4 heightDown = 1 - height/5;
    vec4 res = mix(height, heightDown, waves.x); //se mezclan para la altura final

    vec3 pos = inputPosition;
    pos.y = 3 * res.y; //simula las olas

    float limit = smoothstep(-0.1, 0.1, inputPosition.y); //para q no se salgan las olas
    pos.y *= limit; 

    gl_Position = projectionMatrix * viewMatrix * worldMatrix * vec4(pos, 1.0);


    vec3 camPos =camDir; //para iluminacion
    viewDirec = normalize(camPos - position);

    normal = vec3(worldMatrix * vec4(inputNormal,0));
    vec3 tang = vec3(worldMatrix * vec4(inputTangent, 0));
    vec3 binorm = vec3(worldMatrix * vec4(inputBinormal, 0));

    vec3 nomRes = mix(normal, binorm, waves.x);
    normal = nomRes;

    normal = normalize(normal);
    binorm = normalize(binorm);
    tang = normalize(tang);
} 
