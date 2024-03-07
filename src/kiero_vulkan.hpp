#pragma once

#include <vulkan/vulkan.h>
#include <array>

namespace kiero::vulkan
{
    static constexpr std::array methodsNames{
        "vkCreateInstance", "vkDestroyInstance", "vkEnumeratePhysicalDevices", "vkGetPhysicalDeviceFeatures", "vkGetPhysicalDeviceFormatProperties", "vkGetPhysicalDeviceImageFormatProperties",
        "vkGetPhysicalDeviceProperties", "vkGetPhysicalDeviceQueueFamilyProperties", "vkGetPhysicalDeviceMemoryProperties", "vkGetInstanceProcAddr", "vkGetDeviceProcAddr", "vkCreateDevice",
        "vkDestroyDevice", "vkEnumerateInstanceExtensionProperties", "vkEnumerateDeviceExtensionProperties", "vkEnumerateDeviceLayerProperties", "vkGetDeviceQueue", "vkQueueSubmit", "vkQueueWaitIdle",
        "vkDeviceWaitIdle", "vkAllocateMemory", "vkFreeMemory", "vkMapMemory", "vkUnmapMemory", "vkFlushMappedMemoryRanges", "vkInvalidateMappedMemoryRanges", "vkGetDeviceMemoryCommitment",
        "vkBindBufferMemory", "vkBindImageMemory", "vkGetBufferMemoryRequirements", "vkGetImageMemoryRequirements", "vkGetImageSparseMemoryRequirements", "vkGetPhysicalDeviceSparseImageFormatProperties",
        "vkQueueBindSparse", "vkCreateFence", "vkDestroyFence", "vkResetFences", "vkGetFenceStatus", "vkWaitForFences", "vkCreateSemaphore", "vkDestroySemaphore", "vkCreateEvent", "vkDestroyEvent",
        "vkGetEventStatus", "vkSetEvent", "vkResetEvent", "vkCreateQueryPool", "vkDestroyQueryPool", "vkGetQueryPoolResults", "vkCreateBuffer", "vkDestroyBuffer", "vkCreateBufferView", "vkDestroyBufferView",
        "vkCreateImage", "vkDestroyImage", "vkGetImageSubresourceLayout", "vkCreateImageView", "vkDestroyImageView", "vkCreateShaderModule", "vkDestroyShaderModule", "vkCreatePipelineCache",
        "vkDestroyPipelineCache", "vkGetPipelineCacheData", "vkMergePipelineCaches", "vkCreateGraphicsPipelines", "vkCreateComputePipelines", "vkDestroyPipeline", "vkCreatePipelineLayout",
        "vkDestroyPipelineLayout", "vkCreateSampler", "vkDestroySampler", "vkCreateDescriptorSetLayout", "vkDestroyDescriptorSetLayout", "vkCreateDescriptorPool", "vkDestroyDescriptorPool",
        "vkResetDescriptorPool", "vkAllocateDescriptorSets", "vkFreeDescriptorSets", "vkUpdateDescriptorSets", "vkCreateFramebuffer", "vkDestroyFramebuffer", "vkCreateRenderPass", "vkDestroyRenderPass",
        "vkGetRenderAreaGranularity", "vkCreateCommandPool", "vkDestroyCommandPool", "vkResetCommandPool", "vkAllocateCommandBuffers", "vkFreeCommandBuffers", "vkBeginCommandBuffer", "vkEndCommandBuffer",
        "vkResetCommandBuffer", "vkCmdBindPipeline", "vkCmdSetViewport", "vkCmdSetScissor", "vkCmdSetLineWidth", "vkCmdSetDepthBias", "vkCmdSetBlendConstants", "vkCmdSetDepthBounds",
        "vkCmdSetStencilCompareMask", "vkCmdSetStencilWriteMask", "vkCmdSetStencilReference", "vkCmdBindDescriptorSets", "vkCmdBindIndexBuffer", "vkCmdBindVertexBuffers", "vkCmdDraw", "vkCmdDrawIndexed",
        "vkCmdDrawIndirect", "vkCmdDrawIndexedIndirect", "vkCmdDispatch", "vkCmdDispatchIndirect", "vkCmdCopyBuffer", "vkCmdCopyImage", "vkCmdBlitImage", "vkCmdCopyBufferToImage", "vkCmdCopyImageToBuffer",
        "vkCmdUpdateBuffer", "vkCmdFillBuffer", "vkCmdClearColorImage", "vkCmdClearDepthStencilImage", "vkCmdClearAttachments", "vkCmdResolveImage", "vkCmdSetEvent", "vkCmdResetEvent", "vkCmdWaitEvents",
        "vkCmdPipelineBarrier", "vkCmdBeginQuery", "vkCmdEndQuery", "vkCmdResetQueryPool", "vkCmdWriteTimestamp", "vkCmdCopyQueryPoolResults", "vkCmdPushConstants", "vkCmdBeginRenderPass", "vkCmdNextSubpass",
        "vkCmdEndRenderPass", "vkCmdExecuteCommands"
    };

    static Status init()
    {
        HMODULE libVulkan;
        if ((libVulkan = GetModuleHandle(KIERO_TEXT("vulkan-1.dll"))) == NULL)
        {
            return Status::ModuleNotFoundError;
        }

        g_methodsTable = (uintptr_t*)::calloc(methodsNames.size(), sizeof(uintptr_t));

        for (size_t i = 0; i < methodsNames.size(); i++)
        {
            g_methodsTable[i] = (uintptr_t)::GetProcAddress(libVulkan, methodsNames[i]);
        }

#if KIERO_USE_MINHOOK
        MH_Initialize();
#endif

        g_renderType = RenderType::Vulkan;

        return Status::Success;
    }
}
