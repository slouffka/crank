class Ship : public Entity
{
    public:
        enum Type
        {
            Eagle,
            Raptor
        };

    public:
        explicit            Aircraft(Type type);

    private:
        Type                mType;
};
