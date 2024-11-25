The Any class is designed to store values of any type, leveraging the type erasure technique to encapsulate objects of unknown types and provide a unified interface for interaction.

**Key Aspects of the Implementation**:
1. Polymorphic Base Interface (StorageBase):

A polymorphic base class, `StorageBase`, serves as the foundation for encapsulating the stored data.
`StorageBase` includes a virtual method CreateStorage for creating copies of the contained object and a Info method to provide runtime type information using std::type_index.

2. Type-Specific Storage (`Storage<T>`):

A template class `Storage<T>` inherits from `StorageBase` and wraps a specific type T.
It provides access to the stored value through the Get method and implements the `CreateStorage` method to enable copying of Storage instances.

3. Constructors and Assignment Operators:

The class supports:
A default constructor to initialize an empty `Any` object.
A constructor accepting any type (via a universal reference) that forwards the value to `Storage<T>`.
Copy and move constructors and assignment operators to handle storage correctly:
Copying creates a deep copy of the storage.
Moving transfers ownership, leaving the source object in an empty state.

4. Type Safety (`GetValue<T>`):

The `GetValue<T>` method ensures type safety by comparing the requested type with the stored type using `std::type_index`.
If the types do not match, or if the Any object is empty, the method throws `std::bad_cast`.
State Management:

Methods like `Empty()` and `Clear()` manage the state of the object:
`Empty()` checks if the Any object contains a value.
`Clear()` deallocates the storage and resets the object to an empty state.

5. Additional Features:

A Swap method allows swapping the storage pointers of two Any objects efficiently.

The implementation avoids features like `std::any`, `void*`, and smart pointers, relying instead on custom memory management.