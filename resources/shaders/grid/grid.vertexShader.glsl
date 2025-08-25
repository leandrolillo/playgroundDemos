#version 330

struct GridMatrices {
    mat4 view;
    mat4 proj;
};
uniform GridMatrices matrices;

out GridVertexData {
    vec3 nearPoint;
    vec3 farPoint;
} outputData;


in vec3 position;

vec3 UnprojectPoint(float x, float y, float z, mat4 view, mat4 projection) {
    //mat4 projViewInv = inverse(projection * view);
    //vec4 unprojectedPoint =  projViewInv * vec4(x, y, z, 1.0);

    mat4 viewInv = inverse(view);
    mat4 projInv = inverse(projection);
    vec4 unprojectedPoint =  viewInv * projInv * vec4(x, y, z, 1.0);

    return unprojectedPoint.xyz / unprojectedPoint.w;
}

void main() {
    outputData.nearPoint = UnprojectPoint(position.x, position.y, 0.0, matrices.view, matrices.proj).xyz; // unprojecting on the near plane
    outputData.farPoint = UnprojectPoint(position.x, position.y, 1.0, matrices.view, matrices.proj).xyz; // unprojecting on the far plane
    gl_Position = vec4(position, 1.0); // using directly the clipped coordinates
}
