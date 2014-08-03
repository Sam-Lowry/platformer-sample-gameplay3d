namespace platformer
{
    template<typename NodeUserDataType>
    NodeUserDataType * getNodeUserData(gameplay::Node const * node, int id)
    {
        NodeUserDataType * data = nullptr;

        if(node->getUserPointer())
        {
            INodeUserData * dataInterface = static_cast<INodeUserData*>(node->getUserPointer());

            if(dataInterface->getNodeUserDataId() == id)
            {
                data = static_cast<NodeUserDataType*>(dataInterface);
            }
        }

        return data;
    }
}
